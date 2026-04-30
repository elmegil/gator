/*
 * File:   main.c
 * Author: petehartman
 * 
 * Hook's Gator source code: gate/trigger sequencer using PIC16F15345 (was PIC16F88)
 *
 * Created on June 12, 2018, 9:47 PM
 * Heavily revised May 23 2019
 * Even more heavily revised after May 25 2019, changing chip to get more pins
 * revised/verified for 3.0 PCB and 16F15345 starting Jun 11, 2023
 *  - implemented length of sequence 2023/06/19
 *  - swing is in progress 2023/06/20
 * revised, bugs corrected, etc with assistance of Claude 2026/04/24 - 2026/04/30
 */

// PIC16F15345 Configuration Bit Settings
// config bits set by the configuration bits tool

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with OSCFRQ= 32 MHz and CDIV = 1:1)
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF      // Brown-out reset enable bits (Brown-out reset disabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config BBSIZE = BB512   // Boot Block Size Selection bits (512 words boot block size)
#pragma config BBEN = OFF       // Boot Block Enable bit (Boot Block disabled)
#pragma config SAFEN = OFF      // SAF Enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block Write Protection bit (Application Block not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration Register not write protected)
#pragma config WRTSAF = OFF     // Storage Area Flash Write Protection bit (SAF not write protected)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (High Voltage on MCLR/Vpp must be used for programming)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (UserNVM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic16f15345.h>

// defines for my purposes
// 4051 address pins
#define addrA RC3
#define addrB RC4
#define addrC RC5
// chip select, inhibit is active low, i.e. inh1 = 0 turns off chip 1
// actual chips are active high, but using common collector transistor drivers inverts the logic
#define inh1 RC6
#define inh2 RC7
// inputs
#define runstop RA2
#define updown RA4
#define reset RA5 
#define mode_sw RC1    // read pin for mode state
#define ckout RC0
#define intclk RC2     // this is not ready to use

// ADC acquire bytes in adcon0
// which input, "go" bit, unused, on/off for AD function
// go bit should not be done same time as on/off per data sheet
// so leaving those bits 0 until I explicitly set them
// for 16F15345, bits are different: ADGO bit is in a different place, clock source is in ADCON1
#define rate  0b00110101
#define width 0b00110001
#define steps 0b00111001
#define swing 0b00111101
#define ADGO  0b00000010

// 225 -> 256 with prescaler of 64 gives roughly 1ms resolution on timer 0
#define TM0COUNT 225
//  vvv needed for __delay functions
// 16F88 max is 8MHz, that's what this was originally coded for
// 16F15345 can go up to 32MHz, might try after getting the basics working
#define _XTAL_FREQ 8000000

// no builtin, need this in the ISR so using a macro.  Yes I know this isn't the BEST way to do it
// but I'm only ever going to call this with a single type -- signed char
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// globals
unsigned int  clock0 = 0;  // has to be global for interrupt routine
unsigned int  duty = 0;    // unsigned int here, but calculations done in long to avoid overflow
unsigned int  period = 0;
signed char   row = 0;
signed char   increment = 1;
unsigned char column = 1;  // if mode is 1x16, which column are we in, 1 or 2?
unsigned char col_bits = 1; // just to start, will get assigned 1 2 or 3 later
signed char   last = 8;    // start with full 8 steps
unsigned char mode_state = 0; // 0=2x8, 1=1x16, maintained by main
signed int    offset = 0;   // "swing" will be an offset added or subtracted from period length
signed char   evenodd = 1; // evenodd keeps track of what beat this is, only odd beats get shifted
unsigned char last_clk_state = 0; // previous state of intclk for edge detection
unsigned int  extclk_timeout = 0; // nonzero while external clock is considered present


// have to bite the bullet and make a table lookup for mapping the pot curves.
// note that 1023 is the SLOW end of the pot  So while the data will still map
// 0 -> 1023, 1023 corresponds to the CCW position.
// current table calculated from $y = (($x+325)**6)/(5.85634*10**14); for x = 0 .. 1023
// numbers determined from the intersection of p = ((1023+n)**6)/10230 and p = (n**6)/2
// 6th order equation has a nice curve, 1023 is x max, 10230 is y max (roughly), 
// 0 is x min (so n + 0 in the second equation) 2 is y min (can't use y=0 for obvious reasons)
// if you want to monkey with these values, I recommend wolfram alpha, give it the two
// equations and say equation1 intersects equation2 and it will tell you the point where they intersect
//
// perl code to generate the table:
//
//    #!/usr/bin/perl
//    #
//    # qd to generate Gator input lookup table
//    #
//    print "const int curve[1024] = {";
//    foreach $i (0 .. 63) {
//            foreach $j (0 .. 15) {
//                    $x = ($i * 16 + $j);
//                    $y = (($x+324)**6)/(5.85634*10**14); # 324 gives better result at the high end
//                    printf "%5d,", $y;
//            }
//            print "\n                         ";
//    }
//    print "};\n";

// defines the response curve of the speed pot; values are just clock ticks
// at the fast end probably need to clamp at 8

const unsigned int curve[1024] = {    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,
                             8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,
                             8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,
                             8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,
                             8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,
                             8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    9,    9,
                             9,    9,    9,    9,    9,   10,   10,   10,   10,   10,   10,   10,   11,   11,   11,   11,
                            11,   11,   12,   12,   12,   12,   12,   12,   13,   13,   13,   13,   13,   13,   14,   14,
                            14,   14,   14,   15,   15,   15,   15,   15,   16,   16,   16,   16,   17,   17,   17,   17,
                            17,   18,   18,   18,   18,   19,   19,   19,   19,   20,   20,   20,   20,   21,   21,   21,
                            21,   22,   22,   22,   23,   23,   23,   23,   24,   24,   24,   25,   25,   25,   26,   26,
                            26,   27,   27,   27,   27,   28,   28,   29,   29,   29,   30,   30,   30,   31,   31,   31,
                            32,   32,   32,   33,   33,   34,   34,   34,   35,   35,   36,   36,   36,   37,   37,   38,
                            38,   39,   39,   40,   40,   40,   41,   41,   42,   42,   43,   43,   44,   44,   45,   45,
                            46,   46,   47,   47,   48,   48,   49,   49,   50,   50,   51,   52,   52,   53,   53,   54,
                            54,   55,   56,   56,   57,   57,   58,   59,   59,   60,   61,   61,   62,   63,   63,   64,
                            65,   65,   66,   67,   67,   68,   69,   69,   70,   71,   72,   72,   73,   74,   75,   75,
                            76,   77,   78,   78,   79,   80,   81,   82,   82,   83,   84,   85,   86,   87,   87,   88,
                            89,   90,   91,   92,   93,   94,   95,   96,   96,   97,   98,   99,  100,  101,  102,  103,
                           104,  105,  106,  107,  108,  109,  110,  111,  113,  114,  115,  116,  117,  118,  119,  120,
                           121,  122,  124,  125,  126,  127,  128,  129,  131,  132,  133,  134,  136,  137,  138,  139,
                           141,  142,  143,  145,  146,  147,  149,  150,  151,  153,  154,  155,  157,  158,  160,  161,
                           162,  164,  165,  167,  168,  170,  171,  173,  174,  176,  177,  179,  181,  182,  184,  185,
                           187,  189,  190,  192,  194,  195,  197,  199,  200,  202,  204,  206,  207,  209,  211,  213,
                           215,  216,  218,  220,  222,  224,  226,  228,  230,  232,  233,  235,  237,  239,  241,  243,
                           245,  247,  250,  252,  254,  256,  258,  260,  262,  264,  267,  269,  271,  273,  275,  278,
                           280,  282,  284,  287,  289,  291,  294,  296,  299,  301,  303,  306,  308,  311,  313,  316,
                           318,  321,  323,  326,  329,  331,  334,  336,  339,  342,  344,  347,  350,  353,  355,  358,
                           361,  364,  367,  369,  372,  375,  378,  381,  384,  387,  390,  393,  396,  399,  402,  405,
                           408,  411,  415,  418,  421,  424,  427,  431,  434,  437,  440,  444,  447,  450,  454,  457,
                           461,  464,  468,  471,  475,  478,  482,  485,  489,  493,  496,  500,  504,  507,  511,  515,
                           519,  522,  526,  530,  534,  538,  542,  546,  550,  554,  558,  562,  566,  570,  574,  578,
                           582,  587,  591,  595,  599,  604,  608,  612,  617,  621,  626,  630,  634,  639,  644,  648,
                           653,  657,  662,  667,  671,  676,  681,  686,  690,  695,  700,  705,  710,  715,  720,  725,
                           730,  735,  740,  745,  750,  755,  761,  766,  771,  776,  782,  787,  792,  798,  803,  809,
                           814,  820,  825,  831,  837,  842,  848,  854,  860,  865,  871,  877,  883,  889,  895,  901,
                           907,  913,  919,  925,  931,  938,  944,  950,  956,  963,  969,  976,  982,  989,  995, 1002,
                          1008, 1015, 1021, 1028, 1035, 1042, 1048, 1055, 1062, 1069, 1076, 1083, 1090, 1097, 1104, 1111,
                          1119, 1126, 1133, 1140, 1148, 1155, 1163, 1170, 1177, 1185, 1193, 1200, 1208, 1216, 1223, 1231,
                          1239, 1247, 1255, 1263, 1271, 1279, 1287, 1295, 1303, 1311, 1319, 1328, 1336, 1344, 1353, 1361,
                          1370, 1378, 1387, 1396, 1404, 1413, 1422, 1431, 1440, 1448, 1457, 1466, 1475, 1485, 1494, 1503,
                          1512, 1521, 1531, 1540, 1550, 1559, 1569, 1578, 1588, 1597, 1607, 1617, 1627, 1637, 1646, 1656,
                          1666, 1677, 1687, 1697, 1707, 1717, 1728, 1738, 1748, 1759, 1769, 1780, 1791, 1801, 1812, 1823,
                          1834, 1845, 1856, 1867, 1878, 1889, 1900, 1911, 1922, 1934, 1945, 1957, 1968, 1980, 1991, 2003,
                          2015, 2027, 2038, 2050, 2062, 2074, 2086, 2099, 2111, 2123, 2135, 2148, 2160, 2173, 2185, 2198,
                          2210, 2223, 2236, 2249, 2262, 2275, 2288, 2301, 2314, 2327, 2341, 2354, 2367, 2381, 2394, 2408,
                          2422, 2435, 2449, 2463, 2477, 2491, 2505, 2519, 2533, 2548, 2562, 2576, 2591, 2605, 2620, 2635,
                          2650, 2664, 2679, 2694, 2709, 2724, 2739, 2755, 2770, 2785, 2801, 2816, 2832, 2848, 2863, 2879,
                          2895, 2911, 2927, 2943, 2959, 2975, 2992, 3008, 3025, 3041, 3058, 3074, 3091, 3108, 3125, 3142,
                          3159, 3176, 3193, 3211, 3228, 3245, 3263, 3281, 3298, 3316, 3334, 3352, 3370, 3388, 3406, 3424,
                          3443, 3461, 3480, 3498, 3517, 3536, 3555, 3573, 3592, 3612, 3631, 3650, 3669, 3689, 3708, 3728,
                          3748, 3767, 3787, 3807, 3827, 3847, 3867, 3888, 3908, 3929, 3949, 3970, 3991, 4011, 4032, 4053,
                          4074, 4096, 4117, 4138, 4160, 4181, 4203, 4225, 4247, 4269, 4291, 4313, 4335, 4357, 4380, 4402,
                          4425, 4447, 4470, 4493, 4516, 4539, 4562, 4586, 4609, 4633, 4656, 4680, 4704, 4728, 4752, 4776,
                          4800, 4824, 4849, 4873, 4898, 4922, 4947, 4972, 4997, 5022, 5047, 5073, 5098, 5124, 5149, 5175,
                          5201, 5227, 5253, 5279, 5306, 5332, 5359, 5385, 5412, 5439, 5466, 5493, 5520, 5547, 5575, 5602,
                          5630, 5658, 5685, 5713, 5741, 5770, 5798, 5826, 5855, 5884, 5912, 5941, 5970, 6000, 6029, 6058,
                          6088, 6117, 6147, 6177, 6207, 6237, 6267, 6297, 6328, 6359, 6389, 6420, 6451, 6482, 6513, 6545,
                          6576, 6608, 6639, 6671, 6703, 6735, 6767, 6800, 6832, 6865, 6898, 6930, 6963, 6997, 7030, 7063,
                          7097, 7130, 7164, 7198, 7232, 7266, 7301, 7335, 7370, 7404, 7439, 7474, 7509, 7545, 7580, 7616,
                          7651, 7687, 7723, 7759, 7795, 7832, 7868, 7905, 7942, 7979, 8016, 8053, 8091, 8128, 8166, 8204,
                          8242, 8280, 8318, 8356, 8395, 8434, 8472, 8511, 8551, 8590, 8629, 8669, 8709, 8749, 8789, 8829,
                          8869, 8910, 8950, 8991, 9032, 9073, 9115, 9156, 9198, 9239, 9281, 9323, 9366, 9408, 9451, 9493,
                          9536, 9579, 9622, 9666, 9709, 9753, 9797, 9841, 9885, 9929, 9974,10019,10063,10108,10154,10199,
                         };


void Init_PIC(void) { // commented out bits are from previous part number 16F88
//    OSCCON     = 0b01111100; //osc  8 MHz, primary sys clock, frequency tsable, osc mode defined by FOSC above
    OSCFRQ     = 0b00000011; // 011 is 8MHz clock // 12MHz is 100, 16MHz is 101, 32MHz is 110
    PMD0       = 0b00000110; // disable modules at a higher level; disable NVM & CLKRef
    PMD1       = 0b10000110; // disable NCO, TMR1 & TMR2
    PMD2       = 0b01000111; // disable DAC1, CMP2, CMP1, ZCD (zero cross detection)
    PMD3       = 0b00111111; // disable PWMs and CCPs
    PMD4       = 0b11010001; // disable UARTS, MSSP (serial port), CWG (complementary waveform)
    PMD5       = 0b00011110; // disable config logic cells
    CCP1CON    = 0b00000000; // disable comparator modules
    CCP2CON    = 0b00000000; // disable comparator modules
    SSP1CON1   = 0b00000000; // disable Serial port (bit 5 = 0) (used for I2C/SPI)
    INTCON     = 0b00000001; // disable interrupts; set interrupt to rising edge
    TRISA      = 0b00110100; //port directions; outputs are 0 inputs are 1
    WPUA       = 0b00110100; // weak pull ups for the input bits
    WPUC       = 0b00000100; // RC2 is clock in, pull it up as well
    TRISB      = 0b11110000; 
    TRISC      = 0b00000110; 
    ANSELA     = 0b00000000; //analog select 0 == digital, 1 == analog for A, bits 3,6,7 unused
    ANSELB     = 0b11110000; // for B, bits 0-3 are unused
    ANSELC     = 0b00000000;
    ADCON0     = 0b01000000; // channel sel RC0, disable AD
    ADCON1     = 0b11010000; // rt just, Fosc/8, Vdd as the reference -- note that recommended values are minimum of Fosc/X where X is the MHz of the osc
    PORTA      = 0b00000000; // start up ports at all 0 
    PORTB      = 0b00000000;
    PORTC      = 0b00000000;
}

void Init_interrupts(void) {
    INTCON    &= 0b01111111; // ensure GIE is cleared
    T0CON0     = 0b10000000;  // enable, 8-bit timer, post scaler of 1:1
    T0CON1     = 0b01000110;  // clock source Fosc/4, sync to Fosc/4, prescaler 1:64
    PIE0      &= 0b11011111;  // disable the interrupt
    PIR0      &= 0b11011111;  // clear the flag
    TMR0L      = TM0COUNT;
    PIE0      |= 0b00100000; // enable the interrupt
    INTCON    |= 0b10000000;
}


void __interrupt() ISR(void) {
    TMR0L = TM0COUNT;   // top of ISR improves tempo behavior -- claude advice; previously was at end
    if ((PIE0 & PIR0 & 0b00100000) != 0) { // we have a timer 0 interrupt
        clock0++;
        // normally this would be a bad idea, and the variability of the cycles here is a bit of a problem
        // but this works way better than having all these in the main loop
        // track clock input state even while stopped to avoid spurious edge on start
        unsigned char cur_clk = intclk;
        unsigned char edge = (cur_clk && !last_clk_state);
        last_clk_state = cur_clk;

        if (reset == 1) { // 1 is "not reset"; reset processed outside run/stop so that reset still has an effect when stopped
            if (runstop == 1) { // we are running!
                unsigned char advance_step = 0;

                if (edge) {
                    extclk_timeout = 10000;  // ~10 seconds at 1ms/tick
                    advance_step = 1;
                }
                if (extclk_timeout > 0) extclk_timeout--;

                // internal clock only fires when no external clock present
                if (extclk_timeout == 0 && clock0 >= (unsigned int)((int)period+offset)) {
                    advance_step = 1;
                }

                if (advance_step) {
                    evenodd = (evenodd == 1) ? -1 : 1;
                    clock0 = 0;
                    row += increment;

                    // simpler than first attempt
                    if (mode_state == 0) {      // 2x8 mode
                        if ((row >= last) || (row <= -1)) { // overflow
                            if (row >= last) {
                                row = 0;
                            } else {                        // underflow going backwards
                                row = last-1;
                            }
                        }
                    } else if (column == 1) {       // 1x16 mode, 1st column
                        if (row >= MIN(8,last)) {   // overflow in first column
                            column = (last <= 8) ? 1 : 2;
                            row = 0;
                        } else if (row <= -1) {     // underflow in first column
                            column = (last <= 8) ? 1 : 2;
                            row = (last <= 8) ? (last - 1) : (last - 9);
                        }
                    } else {                        // 1x16 mode, 2nd column
                        if ((8 + row) >= last) {    // overflow in second column
                            column = 1;
                            row = 0;
                        } else if (row <= -1) {     // underflow in second column
                            column = 1;
                            row = 7; // this is correct for going backward into the first column
                        }
                    }
                    // end of column/position manipulation
                    col_bits = (mode_state == 0) ? 3 : column;
                }
                // I'm using transistor buffers which invert the signals
                // so we pre-invert -- ~row -- before adding to PORTC
                unsigned char portc_base = (unsigned char)(((~row & 0x07)<<3) | ((col_bits & 0x03)<<6));
                PORTC = portc_base | (clock0 <= duty ? 1 : 0);  // 1 is gate on, 0 is gate off
            } else { // not running, no clocking!
                col_bits = (mode_state == 0) ? 3 : column;  
                PORTC = (unsigned char)(((~row & 0x07)<<3) | ((col_bits & 0x03)<<6) | 1);   // but leave it lit to ensure we know where we are
                clock0 = 10200; // restart the clock
            }
        } else {  // held in reset, no clocking! and not lit
            PORTC = (unsigned char)(((~row & 0x07)<<3) | ((column & 0x03)<<6) | 0);   // we don't want reset to tell us where we are
            clock0 = 10200; // restart the clock, 10200 will always be > period
            if (updown == 1) { // backward
                if (mode_state ==1 && last > 8) { // 1x16, need second column
                    row = last - 8;
                    column = 2;
                } else {  // 2x8 or 1x16 column one
                    row = last;
                    column = 1;
                }
            } else { //forward
                row = -1;
                column = 1;
            }
            evenodd = 1; 
        }
        PIR0      &= 0b11011111; // reset the interrupt
    }
    INTCON    |= 0b10000000; // set GIE
}

// see page 231 of datasheet (increasing a bit, hoping to get more stable output)
#define AQtime  8
#define TADtime 15

int Acquire(char adcon) {  
    ADCON0 = adcon;      // don't turn on Go bit same time as turn on the ADC
    __delay_us(AQtime);  
    adcon |= ADGO;   
    ADCON0 = adcon;
    __delay_us(TADtime);  // 
    // loop until "go" bit returns to 0
    while ((ADCON0>>1)&1); // wait for DONE#
    return ((((int)ADRESH)<<8) + (int)ADRESL);
}

// notes
// using transistor buffers to get the address and inhibit outputs up to full VCC
// these are inverting buffers so most of the outputs to the 4051s are inverted

void main(void) {
    Init_PIC();
    Init_interrupts();
    for (;;) {
        unsigned int new_period = curve[Acquire(rate)]; // pots are wired with +5V at ccw and gnd at cw
        unsigned int new_duty = (unsigned int)(((unsigned long)(1023 - Acquire(width)) * (unsigned long)new_period)/1023);
        // (swing / 2.5) maps 0 - 409, * (period / 1023) == offset; re-arranging fractions gives (swing * period / 2558)
        signed int new_offset = (signed int)((unsigned long)(1023 - Acquire(swing)) * (unsigned long)new_period / 2558); 
        // precompute adjusted duty for both swing phases outside INTCON -- slow multiply/divide
        // inside the interrupt-disabled block was causing ISR jitter that affected tempo
        signed int pos_offset = new_offset;
        signed int neg_offset = -new_offset;
        unsigned int adj_duty_pos = (unsigned int)((signed int)new_duty + (signed int)((signed long)new_duty * pos_offset / (signed int)new_period));
        unsigned int adj_duty_neg = (unsigned int)((signed int)new_duty + (signed int)((signed long)new_duty * neg_offset / (signed int)new_period));
        INTCON &= 0b01111111; // block interrupts for multi-byte operations
        period = new_period;
        if (evenodd == 1) {
            offset = pos_offset;
            duty = adj_duty_pos;
        } else {
            offset = neg_offset;
            duty = adj_duty_neg;
        }
        signed int eff_period = (signed int)period + offset;
        if ((signed int)duty < 4) duty = 4;
        if ((signed int)duty > (eff_period - 4)) duty = (unsigned int)(eff_period - 4); // minimum 1ms pulse either end with variable duty
        INTCON |= 0b10000000; // unblock interrupts
        // digital inputs
        increment = (updown == 1) ? -1 : 1;

        unsigned char new_mode = mode_sw;
        if (new_mode != mode_state) {
            if (new_mode == 1) column = 1;  // entering 1x16, always start in column 1
            mode_state = new_mode;          // note we don't move the *row*
        }
        // handle steps 
        last = (signed char)(((1023 - Acquire(steps))>>6)+1); // without +1 it never gets to 8 steps
        if (mode_state  == 0) {
            last = last>>1;
        }
    }
}
