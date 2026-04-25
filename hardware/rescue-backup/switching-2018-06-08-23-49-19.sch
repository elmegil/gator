EESchema Schematic File Version 2
LIBS:trigger-seq-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:cmos_ieee
LIBS:petelib
LIBS:switches
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic32mcu
LIBS:altconnector
LIBS:trigger-seq-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 4450 1800 0    60   Output ~ 0
S1
Text HLabel 4450 1900 0    60   Output ~ 0
S2
Text HLabel 4450 2000 0    60   Output ~ 0
S3
Text HLabel 4450 2100 0    60   Output ~ 0
S4
Text HLabel 4450 2200 0    60   Output ~ 0
S5
Text HLabel 4450 2300 0    60   Output ~ 0
S6
Text HLabel 4450 2400 0    60   Output ~ 0
S7
Text HLabel 4450 2500 0    60   Output ~ 0
S8
Text HLabel 4400 4350 0    60   Output ~ 0
S9
Text HLabel 4400 4450 0    60   Output ~ 0
S10
Text HLabel 4400 4550 0    60   Output ~ 0
S11
Text HLabel 4400 4650 0    60   Output ~ 0
S12
Text HLabel 4400 4750 0    60   Output ~ 0
S13
Text HLabel 4400 4850 0    60   Output ~ 0
S14
Text HLabel 4400 4950 0    60   Output ~ 0
S15
Text HLabel 4400 5050 0    60   Output ~ 0
S16
$Comp
L 4051 U6
U 1 1 580D9999
P 5250 4950
F 0 "U6" H 5350 4950 50  0000 C CNN
F 1 "4051" H 5350 4750 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm_LongPads" H 5250 4950 60  0001 C CNN
F 3 "" H 5250 4950 60  0000 C CNN
	1    5250 4950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR077
U 1 1 580D9CC1
P 5950 5550
F 0 "#PWR077" H 5950 5300 50  0001 C CNN
F 1 "GND" H 5950 5400 50  0000 C CNN
F 2 "" H 5950 5550 50  0000 C CNN
F 3 "" H 5950 5550 50  0000 C CNN
	1    5950 5550
	1    0    0    -1  
$EndComp
$Comp
L 4051 U5
U 1 1 580D9F56
P 5250 2400
F 0 "U5" H 5350 2400 50  0000 C CNN
F 1 "4051" H 5350 2200 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm_LongPads" H 5250 2400 60  0001 C CNN
F 3 "" H 5250 2400 60  0000 C CNN
	1    5250 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR078
U 1 1 580DA2D4
P 5950 3050
F 0 "#PWR078" H 5950 2800 50  0001 C CNN
F 1 "GND" H 5950 2900 50  0000 C CNN
F 2 "" H 5950 3050 50  0000 C CNN
F 3 "" H 5950 3050 50  0000 C CNN
	1    5950 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1800 4450 1800
Wire Wire Line
	4550 1900 4450 1900
Wire Wire Line
	4550 2000 4450 2000
Wire Wire Line
	4550 2100 4450 2100
Wire Wire Line
	4550 2200 4450 2200
Wire Wire Line
	4550 2300 4450 2300
Wire Wire Line
	4550 2400 4450 2400
Wire Wire Line
	4550 2500 4450 2500
Wire Wire Line
	4550 4350 4400 4350
Wire Wire Line
	4550 4450 4400 4450
Wire Wire Line
	4550 4550 4400 4550
Wire Wire Line
	4550 4650 4400 4650
Wire Wire Line
	4550 4750 4400 4750
Wire Wire Line
	4550 4850 4400 4850
Wire Wire Line
	4550 4950 4400 4950
Wire Wire Line
	4550 5050 4400 5050
Wire Wire Line
	5950 5550 5950 5450
Wire Wire Line
	5950 3050 5950 2900
$Comp
L CONN_02X08 P7
U 1 1 581296E4
P 5250 3700
F 0 "P7" H 5250 4150 50  0000 C CNN
F 1 "SWITCH_MAIN" V 5250 3700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x08" H 5250 2500 50  0001 C CNN
F 3 "" H 5250 2500 50  0000 C CNN
	1    5250 3700
	1    0    0    -1  
$EndComp
Text HLabel 4800 3350 0    60   Output ~ 0
S1
Text HLabel 4800 3450 0    60   Output ~ 0
S2
Text HLabel 4800 3550 0    60   Output ~ 0
S3
Text HLabel 4800 3650 0    60   Output ~ 0
S4
Text HLabel 4800 3750 0    60   Output ~ 0
S5
Text HLabel 4800 3850 0    60   Output ~ 0
S6
Text HLabel 4800 3950 0    60   Output ~ 0
S7
Text HLabel 4800 4050 0    60   Output ~ 0
S8
Text HLabel 5700 3350 2    60   Output ~ 0
S9
Text HLabel 5700 3450 2    60   Output ~ 0
S10
Text HLabel 5700 3550 2    60   Output ~ 0
S11
Text HLabel 5700 3650 2    60   Output ~ 0
S12
Text HLabel 5700 3750 2    60   Output ~ 0
S13
Text HLabel 5700 3850 2    60   Output ~ 0
S14
Text HLabel 5700 3950 2    60   Output ~ 0
S15
Text HLabel 5700 4050 2    60   Output ~ 0
S16
Wire Wire Line
	4800 3350 5000 3350
Wire Wire Line
	4800 3450 5000 3450
Wire Wire Line
	4800 3550 5000 3550
Wire Wire Line
	4800 3650 5000 3650
Wire Wire Line
	4800 3750 5000 3750
Wire Wire Line
	4800 3850 5000 3850
Wire Wire Line
	4800 3950 5000 3950
Wire Wire Line
	4800 4050 5000 4050
Wire Wire Line
	5500 3350 5700 3350
Wire Wire Line
	5500 3450 5700 3450
Wire Wire Line
	5500 3550 5700 3550
Wire Wire Line
	5500 3650 5700 3650
Wire Wire Line
	5500 3750 5700 3750
Wire Wire Line
	5500 3850 5700 3850
Wire Wire Line
	5500 3950 5700 3950
Wire Wire Line
	5500 4050 5700 4050
Text HLabel 6100 1800 2    60   Input ~ 0
ckbuf
Text HLabel 6100 4350 2    60   Input ~ 0
ckbuf
Wire Wire Line
	5950 1800 6100 1800
Wire Wire Line
	5950 4350 6100 4350
Text HLabel 4450 2700 0    60   Input ~ 0
inh1
Text HLabel 4400 5250 0    60   Input ~ 0
inh2
Text HLabel 4450 2800 0    60   Input ~ 0
adr1
Text HLabel 4450 2900 0    60   Input ~ 0
adr2
Text HLabel 4450 3000 0    60   Input ~ 0
adr3
Text HLabel 4400 5350 0    60   Input ~ 0
adr1
Text HLabel 4400 5450 0    60   Input ~ 0
adr2
Text HLabel 4400 5550 0    60   Input ~ 0
adr3
Wire Wire Line
	4450 2700 4550 2700
Wire Wire Line
	4450 2800 4550 2800
Wire Wire Line
	4450 2900 4550 2900
Wire Wire Line
	4450 3000 4550 3000
Wire Wire Line
	4400 5250 4550 5250
Wire Wire Line
	4400 5350 4550 5350
Wire Wire Line
	4400 5450 4550 5450
Wire Wire Line
	4400 5550 4550 5550
$EndSCHEMATC
