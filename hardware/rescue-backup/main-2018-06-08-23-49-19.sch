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
Sheet 3 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 7150 2900 1350 2200
U 58097C7C
F0 "outputs" 60
F1 "outputs.sch" 60
F2 "S1" I L 7150 3050 60 
F3 "S2" I L 7150 3150 60 
F4 "S3" I L 7150 3250 60 
F5 "S4" I L 7150 3350 60 
F6 "S5" I L 7150 3450 60 
F7 "S6" I L 7150 3550 60 
F8 "S7" I L 7150 3650 60 
F9 "S8" I L 7150 3750 60 
F10 "S9" I L 7150 3850 60 
F11 "S10" I L 7150 3950 60 
F12 "S11" I L 7150 4050 60 
F13 "S12" I L 7150 4150 60 
F14 "S13" I L 7150 4250 60 
F15 "S14" I L 7150 4350 60 
F16 "S15" I L 7150 4450 60 
F17 "S16" I L 7150 4550 60 
$EndSheet
Wire Wire Line
	6550 3050 7150 3050
Wire Wire Line
	6550 3150 7150 3150
Wire Wire Line
	6550 3250 7150 3250
Wire Wire Line
	6550 3350 7150 3350
Wire Wire Line
	6550 3450 7150 3450
Wire Wire Line
	6550 3550 7150 3550
Wire Wire Line
	6550 3650 7150 3650
Wire Wire Line
	6550 3750 7150 3750
Wire Wire Line
	6550 3850 7150 3850
Wire Wire Line
	6550 3950 7150 3950
Wire Wire Line
	6550 4050 7150 4050
Wire Wire Line
	6550 4150 7150 4150
Wire Wire Line
	6550 4250 7150 4250
Wire Wire Line
	6550 4350 7150 4350
Wire Wire Line
	6550 4450 7150 4450
Wire Wire Line
	6550 4550 7150 4550
$Sheet
S 2050 2900 1850 2200
U 58097C50
F0 "clock" 60
F1 "clock.sch" 60
F2 "adr1" O R 3900 3050 60 
F3 "adr2" O R 3900 3150 60 
F4 "adr3" O R 3900 3250 60 
F5 "inh1" O R 3900 3450 60 
F6 "inh2" O R 3900 3550 60 
F7 "ckbuf" O R 3900 3700 60 
$EndSheet
Wire Wire Line
	3900 3050 4700 3050
Wire Wire Line
	3900 3150 4700 3150
Wire Wire Line
	3900 3250 4700 3250
Wire Wire Line
	3900 3450 4700 3450
Wire Wire Line
	3900 3550 4700 3550
Wire Wire Line
	3900 3700 4700 3700
$Sheet
S 4700 2900 1850 2200
U 58097C5F
F0 "switching" 60
F1 "switching.sch" 60
F2 "S1" O R 6550 3050 60 
F3 "S2" O R 6550 3150 60 
F4 "S3" O R 6550 3250 60 
F5 "S4" O R 6550 3350 60 
F6 "S5" O R 6550 3450 60 
F7 "S6" O R 6550 3550 60 
F8 "S7" O R 6550 3650 60 
F9 "S8" O R 6550 3750 60 
F10 "S9" O R 6550 3850 60 
F11 "S10" O R 6550 3950 60 
F12 "S11" O R 6550 4050 60 
F13 "S12" O R 6550 4150 60 
F14 "S13" O R 6550 4250 60 
F15 "S14" O R 6550 4350 60 
F16 "S15" O R 6550 4450 60 
F17 "S16" O R 6550 4550 60 
F18 "inh1" I L 4700 3450 60 
F19 "inh2" I L 4700 3550 60 
F20 "adr1" I L 4700 3050 60 
F21 "adr2" I L 4700 3150 60 
F22 "adr3" I L 4700 3250 60 
F23 "ckbuf" I L 4700 3700 60 
$EndSheet
$EndSCHEMATC
