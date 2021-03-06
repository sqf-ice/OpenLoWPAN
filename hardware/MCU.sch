EESchema Schematic File Version 2
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
LIBS:stm32
LIBS:rfcom
LIBS:texas
LIBS:dongle
LIBS:switches
LIBS:dongle-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title "Microcontroller and power"
Date "2017-05-17"
Rev "A"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L STM32F072C8Tx U?
U 1 1 57A47526
P 5800 4400
AR Path="/57A47526" Ref="U?"  Part="1" 
AR Path="/57A46712/57A47526" Ref="U2"  Part="1" 
F 0 "U2" H 2150 6250 50  0000 C CNN
F 1 "STM32F072C8Tx" H 2450 6150 50  0000 C CNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 2300 6050 50  0001 C CNN
F 3 "" H 5800 4400 50  0000 C CNN
	1    5800 4400
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 57A47534
P 1400 3400
F 0 "R1" H 1470 3446 50  0000 L CNN
F 1 "100k" H 1470 3355 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 1330 3400 50  0001 C CNN
F 3 "" H 1400 3400 50  0000 C CNN
	1    1400 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 57A4753B
P 1400 3600
F 0 "#PWR01" H 1400 3350 50  0001 C CNN
F 1 "GND" H 1405 3427 50  0001 C CNN
F 2 "" H 1400 3600 50  0000 C CNN
F 3 "" H 1400 3600 50  0000 C CNN
	1    1400 3600
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR02
U 1 1 57A47541
P 1050 3200
F 0 "#PWR02" H 1050 3050 50  0001 C CNN
F 1 "VCC" V 1050 3300 50  0000 L CNN
F 2 "" H 1050 3200 50  0000 C CNN
F 3 "" H 1050 3200 50  0000 C CNN
	1    1050 3200
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR03
U 1 1 57A47547
P 1950 3400
F 0 "#PWR03" H 1950 3250 50  0001 C CNN
F 1 "VCC" V 1950 3500 50  0000 L CNN
F 2 "" H 1950 3400 50  0000 C CNN
F 3 "" H 1950 3400 50  0000 C CNN
	1    1950 3400
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X05 P2
U 1 1 57A4754D
P 10550 6100
F 0 "P2" H 10628 6141 50  0000 L CNN
F 1 "SWD" H 10628 6050 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05" H 10550 6100 50  0001 C CNN
F 3 "" H 10550 6100 50  0000 C CNN
	1    10550 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 57A47554
P 10300 6100
F 0 "#PWR04" H 10300 5850 50  0001 C CNN
F 1 "GND" H 10305 5927 50  0001 C CNN
F 2 "" H 10300 6100 50  0000 C CNN
F 3 "" H 10300 6100 50  0000 C CNN
	1    10300 6100
	0    1    1    0   
$EndComp
Text Label 10100 6300 0    50   ~ 0
~RESET
Text Label 1750 3000 0    50   ~ 0
~RESET
Text Label 10100 6000 0    50   ~ 0
SWCLK
Text Label 10100 6200 0    50   ~ 0
SWDIO
$Comp
L VCC #PWR05
U 1 1 57A4755E
P 10300 5900
F 0 "#PWR05" H 10300 5750 50  0001 C CNN
F 1 "VCC" V 10300 6000 50  0000 L CNN
F 2 "" H 10300 5900 50  0000 C CNN
F 3 "" H 10300 5900 50  0000 C CNN
	1    10300 5900
	0    -1   -1   0   
$EndComp
$Comp
L USB_A P1
U 1 1 57A47564
P 10750 5300
F 0 "P1" H 10750 5650 50  0000 C CNN
F 1 "USB" H 10900 4950 50  0000 C CNN
F 2 "dongle:USBA-M-SM" V 10700 5200 50  0001 C CNN
F 3 "" V 10700 5200 50  0000 C CNN
	1    10750 5300
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 57A47571
P 10850 5800
F 0 "#PWR06" H 10850 5550 50  0001 C CNN
F 1 "GND" H 10855 5627 50  0001 C CNN
F 2 "" H 10850 5800 50  0000 C CNN
F 3 "" H 10850 5800 50  0000 C CNN
	1    10850 5800
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR07
U 1 1 57A47577
P 10400 5100
F 0 "#PWR07" H 10400 4950 50  0001 C CNN
F 1 "+5V" V 10400 5200 50  0000 L CNN
F 2 "" H 10400 5100 50  0000 C CNN
F 3 "" H 10400 5100 50  0000 C CNN
	1    10400 5100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1300 3200 2000 3200
Wire Wire Line
	1400 3250 1400 3200
Connection ~ 1400 3200
Wire Wire Line
	1400 3600 1400 3550
Wire Wire Line
	1050 3200 1100 3200
Wire Wire Line
	1950 3400 2000 3400
Wire Wire Line
	10350 6100 10300 6100
Wire Wire Line
	10000 6200 10350 6200
Wire Wire Line
	10350 6300 10100 6300
Wire Wire Line
	1650 3000 2000 3000
Wire Wire Line
	10300 5900 10350 5900
Wire Wire Line
	9900 6000 10350 6000
Wire Wire Line
	9900 5700 9900 6000
Wire Wire Line
	9900 5700 9600 5700
Wire Wire Line
	10000 5600 10000 6200
Wire Wire Line
	10000 5600 9600 5600
Wire Wire Line
	9600 5400 10450 5400
Wire Wire Line
	9600 5500 10400 5500
$Comp
L VCC #PWR08
U 1 1 57A47593
P 5700 2400
F 0 "#PWR08" H 5700 2250 50  0001 C CNN
F 1 "VCC" H 5700 2550 50  0000 C CNN
F 2 "" H 5700 2400 50  0000 C CNN
F 3 "" H 5700 2400 50  0000 C CNN
	1    5700 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2500 5600 2450
Wire Wire Line
	5600 2450 5900 2450
Wire Wire Line
	5900 2450 5900 2500
Wire Wire Line
	5800 2500 5800 2450
Connection ~ 5800 2450
Wire Wire Line
	5700 2400 5700 2500
Connection ~ 5700 2450
$Comp
L GND #PWR09
U 1 1 57A475A3
P 5700 6300
F 0 "#PWR09" H 5700 6050 50  0001 C CNN
F 1 "GND" H 5705 6127 50  0001 C CNN
F 2 "" H 5700 6300 50  0000 C CNN
F 3 "" H 5700 6300 50  0000 C CNN
	1    5700 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 6200 5600 6250
Wire Wire Line
	5600 6250 5900 6250
Wire Wire Line
	5900 6250 5900 6200
Wire Wire Line
	5800 6200 5800 6250
Connection ~ 5800 6250
Wire Wire Line
	5700 6200 5700 6300
Connection ~ 5700 6250
$Comp
L C C4
U 1 1 57A47DF7
P 4100 1450
F 0 "C4" H 4100 1550 50  0000 L CNN
F 1 "100nF" H 4100 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4138 1300 50  0001 C CNN
F 3 "" H 4100 1450 50  0000 C CNN
	1    4100 1450
	1    0    0    -1  
$EndComp
$Comp
L CP1 C3
U 1 1 57A47DFE
P 3850 1450
F 0 "C3" H 3850 1550 50  0000 L CNN
F 1 "4.7µF" H 3850 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3850 1450 50  0001 C CNN
F 3 "" H 3850 1450 50  0000 C CNN
	1    3850 1450
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 57A47E05
P 4350 1450
F 0 "C5" H 4350 1550 50  0000 L CNN
F 1 "100nF" H 4350 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4388 1300 50  0001 C CNN
F 3 "" H 4350 1450 50  0000 C CNN
	1    4350 1450
	1    0    0    -1  
$EndComp
$Comp
L CP1 C6
U 1 1 57A47E0C
P 4850 1450
F 0 "C6" H 4850 1550 50  0000 L CNN
F 1 "4.7µF" H 4850 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4850 1450 50  0001 C CNN
F 3 "" H 4850 1450 50  0000 C CNN
	1    4850 1450
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 57A47E13
P 5100 1450
F 0 "C7" H 5100 1550 50  0000 L CNN
F 1 "100nF" H 5100 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 5138 1300 50  0001 C CNN
F 3 "" H 5100 1450 50  0000 C CNN
	1    5100 1450
	1    0    0    -1  
$EndComp
$Comp
L CP1 C8
U 1 1 57A47E1A
P 5600 1450
F 0 "C8" H 5600 1550 50  0000 L CNN
F 1 "4.7µF" H 5600 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5600 1450 50  0001 C CNN
F 3 "" H 5600 1450 50  0000 C CNN
	1    5600 1450
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 57A47E21
P 5850 1450
F 0 "C9" H 5850 1550 50  0000 L CNN
F 1 "100nF" H 5850 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 5888 1300 50  0001 C CNN
F 3 "" H 5850 1450 50  0000 C CNN
	1    5850 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 1250 4350 1300
Wire Wire Line
	3850 1250 4350 1250
Wire Wire Line
	3850 1250 3850 1300
Wire Wire Line
	4100 1200 4100 1300
Connection ~ 4100 1250
Wire Wire Line
	3850 1600 3850 1650
Wire Wire Line
	3850 1650 4350 1650
Wire Wire Line
	4350 1650 4350 1600
Wire Wire Line
	4100 1600 4100 1700
Connection ~ 4100 1650
Wire Wire Line
	4850 1200 4850 1300
Wire Wire Line
	4850 1250 5100 1250
Wire Wire Line
	5100 1250 5100 1300
Wire Wire Line
	4850 1600 4850 1700
Wire Wire Line
	4850 1650 5100 1650
Wire Wire Line
	5100 1650 5100 1600
Wire Wire Line
	5600 1200 5600 1300
Wire Wire Line
	5600 1250 5850 1250
Wire Wire Line
	5850 1250 5850 1300
Wire Wire Line
	5600 1600 5600 1700
Wire Wire Line
	5600 1650 5850 1650
Wire Wire Line
	5850 1650 5850 1600
$Comp
L VCC #PWR010
U 1 1 57A47E3E
P 4100 1200
F 0 "#PWR010" H 4100 1050 50  0001 C CNN
F 1 "VCC" H 4100 1350 50  0000 C CNN
F 2 "" H 4100 1200 50  0000 C CNN
F 3 "" H 4100 1200 50  0000 C CNN
	1    4100 1200
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR011
U 1 1 57A47E44
P 4850 1200
F 0 "#PWR011" H 4850 1050 50  0001 C CNN
F 1 "VCC" H 4850 1350 50  0000 C CNN
F 2 "" H 4850 1200 50  0000 C CNN
F 3 "" H 4850 1200 50  0000 C CNN
	1    4850 1200
	1    0    0    -1  
$EndComp
Connection ~ 4850 1250
$Comp
L VCC #PWR012
U 1 1 57A47E4B
P 5600 1200
F 0 "#PWR012" H 5600 1050 50  0001 C CNN
F 1 "VCC" H 5600 1350 50  0000 C CNN
F 2 "" H 5600 1200 50  0000 C CNN
F 3 "" H 5600 1200 50  0000 C CNN
	1    5600 1200
	1    0    0    -1  
$EndComp
Connection ~ 5600 1250
$Comp
L GND #PWR013
U 1 1 57A47E52
P 4100 1700
F 0 "#PWR013" H 4100 1450 50  0001 C CNN
F 1 "GND" H 4105 1527 50  0001 C CNN
F 2 "" H 4100 1700 50  0000 C CNN
F 3 "" H 4100 1700 50  0000 C CNN
	1    4100 1700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 57A47E58
P 4850 1700
F 0 "#PWR014" H 4850 1450 50  0001 C CNN
F 1 "GND" H 4855 1527 50  0001 C CNN
F 2 "" H 4850 1700 50  0000 C CNN
F 3 "" H 4850 1700 50  0000 C CNN
	1    4850 1700
	1    0    0    -1  
$EndComp
Connection ~ 4850 1650
$Comp
L GND #PWR015
U 1 1 57A47E5F
P 5600 1700
F 0 "#PWR015" H 5600 1450 50  0001 C CNN
F 1 "GND" H 5605 1527 50  0001 C CNN
F 2 "" H 5600 1700 50  0000 C CNN
F 3 "" H 5600 1700 50  0000 C CNN
	1    5600 1700
	1    0    0    -1  
$EndComp
Connection ~ 5600 1650
$Comp
L NCP1117ST33T3G U1
U 1 1 57A47E66
P 2550 1250
F 0 "U1" H 2550 1500 50  0000 C CNN
F 1 "NCP1117ST33T3G" H 2550 1400 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-223" H 2550 1250 50  0001 C CNN
F 3 "" H 2550 1250 50  0000 C CNN
	1    2550 1250
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR016
U 1 1 57A47E6D
P 1650 1250
F 0 "#PWR016" H 1650 1100 50  0001 C CNN
F 1 "+5V" V 1650 1350 50  0000 L CNN
F 2 "" H 1650 1250 50  0000 C CNN
F 3 "" H 1650 1250 50  0000 C CNN
	1    1650 1250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1650 1250 2250 1250
$Comp
L GND #PWR017
U 1 1 57A47E74
P 2550 1650
F 0 "#PWR017" H 2550 1400 50  0001 C CNN
F 1 "GND" H 2555 1477 50  0001 C CNN
F 2 "" H 2550 1650 50  0000 C CNN
F 3 "" H 2550 1650 50  0000 C CNN
	1    2550 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 1550 2550 1650
$Comp
L CP1 C1
U 1 1 57A47E7B
P 1800 1450
F 0 "C1" H 1800 1550 50  0000 L CNN
F 1 "4.7µF" H 1800 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1800 1450 50  0001 C CNN
F 3 "" H 1800 1450 50  0000 C CNN
	1    1800 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1300 1800 1250
Connection ~ 1800 1250
$Comp
L GND #PWR018
U 1 1 57A47E84
P 1800 1650
F 0 "#PWR018" H 1800 1400 50  0001 C CNN
F 1 "GND" H 1805 1477 50  0001 C CNN
F 2 "" H 1800 1650 50  0000 C CNN
F 3 "" H 1800 1650 50  0000 C CNN
	1    1800 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1650 1800 1600
$Comp
L VCC #PWR019
U 1 1 57A47E8B
P 3250 1250
F 0 "#PWR019" H 3250 1100 50  0001 C CNN
F 1 "VCC" V 3250 1450 50  0000 C CNN
F 2 "" H 3250 1250 50  0000 C CNN
F 3 "" H 3250 1250 50  0000 C CNN
	1    3250 1250
	0    1    1    0   
$EndComp
Wire Wire Line
	2850 1250 3000 1250
$Comp
L C C2
U 1 1 57A4804B
P 2050 1450
F 0 "C2" H 2050 1550 50  0000 L CNN
F 1 "100nF" H 2050 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 2088 1300 50  0001 C CNN
F 3 "" H 2050 1450 50  0000 C CNN
	1    2050 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 1300 2050 1250
Connection ~ 2050 1250
$Comp
L GND #PWR020
U 1 1 57A48132
P 2050 1650
F 0 "#PWR020" H 2050 1400 50  0001 C CNN
F 1 "GND" H 2055 1477 50  0001 C CNN
F 2 "" H 2050 1650 50  0000 C CNN
F 3 "" H 2050 1650 50  0000 C CNN
	1    2050 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 1600 2050 1650
Text Label 10250 5500 0    50   ~ 0
D+
Text Label 10250 5400 0    50   ~ 0
D-
$Comp
L GND #PWR021
U 1 1 57A494EE
P 1100 3000
F 0 "#PWR021" H 1100 2750 50  0001 C CNN
F 1 "GND" H 1105 2827 50  0001 C CNN
F 2 "" H 1100 3000 50  0000 C CNN
F 3 "" H 1100 3000 50  0000 C CNN
	1    1100 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	1100 3000 1250 3000
$Comp
L C C10
U 1 1 57A49632
P 1450 2700
F 0 "C10" V 1400 2650 50  0000 R CNN
F 1 "100nF" V 1400 2750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 1488 2550 50  0001 C CNN
F 3 "" H 1450 2700 50  0000 C CNN
	1    1450 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	1300 2700 1150 2700
Wire Wire Line
	1150 2700 1150 3000
Connection ~ 1150 3000
Wire Wire Line
	1600 2700 1750 2700
Wire Wire Line
	1750 2700 1750 3000
Connection ~ 1750 3000
Text HLabel 9750 5100 2    50   Output ~ 0
RF_MCO
Wire Wire Line
	9600 5100 9750 5100
Text HLabel 9750 4900 2    50   Input ~ 0
RF_MISO
Text HLabel 9750 5000 2    50   Output ~ 0
RF_MOSI
Wire Wire Line
	9750 4700 9600 4700
Wire Wire Line
	9600 4800 9750 4800
Wire Wire Line
	9750 4900 9600 4900
Wire Wire Line
	9600 5000 9750 5000
Text HLabel 1900 4300 0    50   Output ~ 0
RF_nPDN
Wire Wire Line
	2000 4300 1900 4300
Text HLabel 1900 4400 0    50   Output ~ 0
RF_nRST
Wire Wire Line
	2000 4400 1900 4400
Wire Wire Line
	3200 1250 3250 1250
Text HLabel 1900 5100 0    50   BiDi ~ 0
RF_GPIO0
Text HLabel 1900 5200 0    50   BiDi ~ 0
RF_GPIO1
Text HLabel 1900 5800 0    50   BiDi ~ 0
RF_GPIO2
Text HLabel 1900 5700 0    50   BiDi ~ 0
RF_GPIO3
Text HLabel 1900 5600 0    50   BiDi ~ 0
RF_GPIO4
Text HLabel 1900 5500 0    50   BiDi ~ 0
RF_GPIO5
Wire Wire Line
	2000 5500 1900 5500
Wire Wire Line
	1900 5600 2000 5600
Wire Wire Line
	2000 5700 1900 5700
Wire Wire Line
	1900 5800 2000 5800
$Comp
L LED D1
U 1 1 57A4DF3D
P 1200 5300
F 0 "D1" H 1050 5400 50  0000 L CNN
F 1 "RX" H 1250 5250 50  0000 L CNN
F 2 "LEDs:LED_0805" H 1200 5300 50  0001 C CNN
F 3 "" H 1200 5300 50  0000 C CNN
	1    1200 5300
	1    0    0    -1  
$EndComp
$Comp
L LED D2
U 1 1 57A4E252
P 1200 5550
F 0 "D2" H 1050 5650 50  0000 L CNN
F 1 "TX" H 1250 5500 50  0000 L CNN
F 2 "LEDs:LED_0805" H 1200 5550 50  0001 C CNN
F 3 "" H 1200 5550 50  0000 C CNN
	1    1200 5550
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 57A4E8A4
P 800 5300
F 0 "R2" V 700 5300 50  0000 C CNN
F 1 "1k" V 800 5300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 730 5300 50  0001 C CNN
F 3 "" H 800 5300 50  0000 C CNN
	1    800  5300
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 57A4EA55
P 800 5550
F 0 "R3" V 700 5550 50  0000 C CNN
F 1 "1k" V 800 5550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 730 5550 50  0001 C CNN
F 3 "" H 800 5550 50  0000 C CNN
	1    800  5550
	0    1    1    0   
$EndComp
Wire Wire Line
	950  5300 1050 5300
Wire Wire Line
	950  5550 1050 5550
$Comp
L GND #PWR022
U 1 1 57A4EE60
P 600 5300
F 0 "#PWR022" H 600 5050 50  0001 C CNN
F 1 "GND" H 605 5127 50  0001 C CNN
F 2 "" H 600 5300 50  0000 C CNN
F 3 "" H 600 5300 50  0000 C CNN
	1    600  5300
	0    1    1    0   
$EndComp
$Comp
L GND #PWR023
U 1 1 57A4EED9
P 600 5550
F 0 "#PWR023" H 600 5300 50  0001 C CNN
F 1 "GND" H 605 5377 50  0001 C CNN
F 2 "" H 600 5550 50  0000 C CNN
F 3 "" H 600 5550 50  0000 C CNN
	1    600  5550
	0    1    1    0   
$EndComp
Wire Wire Line
	600  5550 650  5550
Wire Wire Line
	650  5300 600  5300
$Comp
L Jumper_NO_Small JP2
U 1 1 57A5C19F
P 1200 3200
F 0 "JP2" H 1200 3293 50  0000 C CNN
F 1 "Jumper_NO_Small" H 1210 3140 50  0001 C CNN
F 2 "Resistors_SMD:R_0805" H 1200 3200 50  0001 C CNN
F 3 "" H 1200 3200 50  0000 C CNN
	1    1200 3200
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NC_Small JP1
U 1 1 57A5C641
P 3100 1250
F 0 "JP1" H 3100 1371 50  0000 C CNN
F 1 "Jumper_NC_Small" H 3110 1190 50  0001 C CNN
F 2 "Resistors_SMD:R_0805" H 3100 1250 50  0001 C CNN
F 3 "" H 3100 1250 50  0000 C CNN
	1    3100 1250
	1    0    0    -1  
$EndComp
Text HLabel 9750 4700 2    50   Output ~ 0
RF_nCS
Text HLabel 9750 4800 2    50   Output ~ 0
RF_SCK
Wire Wire Line
	2000 5100 1900 5100
Wire Wire Line
	1900 5200 2000 5200
Wire Wire Line
	1350 5300 2000 5300
Wire Wire Line
	1400 5550 1400 5400
Wire Wire Line
	1400 5400 2000 5400
Wire Wire Line
	1400 5550 1350 5550
$Comp
L SW_Push SW1
U 1 1 591C7D72
P 1450 3000
F 0 "SW1" H 1250 3100 50  0000 C CNN
F 1 "RESET" H 1600 3100 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_FSMSM" H 1450 3200 50  0001 C CNN
F 3 "" H 1450 3200 50  0001 C CNN
	1    1450 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10850 5800 10850 5700
Wire Wire Line
	10750 5700 10750 5750
Wire Wire Line
	10750 5750 10850 5750
Connection ~ 10850 5750
Wire Wire Line
	10400 5500 10400 5300
Wire Wire Line
	10400 5300 10450 5300
Wire Wire Line
	10450 5100 10400 5100
$EndSCHEMATC
