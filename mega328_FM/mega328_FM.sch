EESchema Schematic File Version 4
LIBS:mega328_FM-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5C71421F
P 2900 3150
F 0 "A1" H 2150 2200 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 2450 2100 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 3050 2200 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 2900 2150 50  0001 C CNN
	1    2900 3150
	1    0    0    -1  
$EndComp
$Comp
L Interface_Optical:TSOP17xx U1
U 1 1 5C71432D
P 1400 2750
F 0 "U1" H 1387 3175 50  0000 C CNN
F 1 "TSOP17xx" H 1387 3084 50  0000 C CNN
F 2 "OptoDevice:Vishay_CAST-3Pin" H 1350 2375 50  0001 C CNN
F 3 "http://www.micropik.com/PDF/tsop17xx.pdf" H 2050 3050 50  0001 C CNN
	1    1400 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5C7143FA
P 1850 3050
F 0 "#PWR0101" H 1850 2800 50  0001 C CNN
F 1 "GND" H 1855 2877 50  0000 C CNN
F 2 "" H 1850 3050 50  0001 C CNN
F 3 "" H 1850 3050 50  0001 C CNN
	1    1850 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 3050 1850 2950
Wire Wire Line
	1850 2950 1800 2950
Wire Wire Line
	2900 4150 3000 4150
$Comp
L power:GND #PWR0102
U 1 1 5C714452
P 3000 4250
F 0 "#PWR0102" H 3000 4000 50  0001 C CNN
F 1 "GND" H 3005 4077 50  0000 C CNN
F 2 "" H 3000 4250 50  0001 C CNN
F 3 "" H 3000 4250 50  0001 C CNN
	1    3000 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 4250 3000 4150
Connection ~ 3000 4150
$Comp
L power:+5V #PWR0104
U 1 1 5C714565
P 1850 2450
F 0 "#PWR0104" H 1850 2300 50  0001 C CNN
F 1 "+5V" H 1865 2623 50  0000 C CNN
F 2 "" H 1850 2450 50  0001 C CNN
F 3 "" H 1850 2450 50  0001 C CNN
	1    1850 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 2550 1850 2550
Wire Wire Line
	1850 2550 1850 2450
Wire Wire Line
	1800 2750 2000 2750
$Comp
L RDA5807M_module:RDA5807M U2
U 1 1 5C715067
P 4750 3800
F 0 "U2" H 4800 4297 60  0000 C CNN
F 1 "RDA5807M" H 4800 4191 60  0000 C CNN
F 2 "RDA5807M:RDA5807M_SMD" H 4800 4191 60  0001 C CNN
F 3 "" H 4900 3800 60  0000 C CNN
	1    4750 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3950 5250 3950
$Comp
L Device:L_Small L1
U 1 1 5C715651
P 4000 3950
F 0 "L1" V 4100 4000 50  0000 C CNN
F 1 "1u" V 4100 3850 50  0000 C CNN
F 2 "Inductor_THT:L_Axial_L7.0mm_D3.3mm_P10.16mm_Horizontal_Fastron_MICC" H 4000 3950 50  0001 C CNN
F 3 "~" H 4000 3950 50  0001 C CNN
	1    4000 3950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4100 3950 4150 3950
$Comp
L Device:CP_Small C1
U 1 1 5C715C5A
P 4150 4100
F 0 "C1" H 3950 4100 50  0000 L CNN
F 1 "10u" H 3950 4000 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 4150 4100 50  0001 C CNN
F 3 "~" H 4150 4100 50  0001 C CNN
	1    4150 4100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 5C715D2B
P 4300 4100
F 0 "C2" H 4400 4100 50  0000 L CNN
F 1 "100n" H 4400 4000 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P5.00mm" H 4300 4100 50  0001 C CNN
F 3 "~" H 4300 4100 50  0001 C CNN
	1    4300 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 4250 4150 4200
Wire Wire Line
	4300 4250 4300 4200
Wire Wire Line
	4300 4000 4300 3950
Connection ~ 4300 3950
Wire Wire Line
	4300 3950 4350 3950
Wire Wire Line
	4150 4000 4150 3950
Connection ~ 4150 3950
Wire Wire Line
	4150 3950 4300 3950
Wire Wire Line
	4150 4250 4300 4250
$Comp
L power:GND #PWR0103
U 1 1 5C716868
P 4300 4300
F 0 "#PWR0103" H 4300 4050 50  0001 C CNN
F 1 "GND" H 4305 4127 50  0000 C CNN
F 2 "" H 4300 4300 50  0001 C CNN
F 3 "" H 4300 4300 50  0001 C CNN
	1    4300 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 4300 4300 4250
Connection ~ 4300 4250
$Comp
L power:GND #PWR0105
U 1 1 5C716C82
P 5350 4300
F 0 "#PWR0105" H 5350 4050 50  0001 C CNN
F 1 "GND" H 5355 4127 50  0000 C CNN
F 2 "" H 5350 4300 50  0001 C CNN
F 3 "" H 5350 4300 50  0001 C CNN
	1    5350 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3950 5350 4300
$Comp
L Device:R_Small R2
U 1 1 5C716F16
P 4100 3650
F 0 "R2" V 4060 3515 50  0000 C CNN
F 1 "3k3" V 4060 3790 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 4100 3650 50  0001 C CNN
F 3 "~" H 4100 3650 50  0001 C CNN
	1    4100 3650
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R1
U 1 1 5C717950
P 4100 3550
F 0 "R1" V 4060 3415 50  0000 C CNN
F 1 "3k3" V 4060 3690 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 4100 3550 50  0001 C CNN
F 3 "~" H 4100 3550 50  0001 C CNN
	1    4100 3550
	0    1    1    0   
$EndComp
Wire Wire Line
	4200 3550 4350 3550
Wire Wire Line
	4200 3650 4350 3650
Wire Wire Line
	4000 3550 3400 3550
Wire Wire Line
	3400 3650 4000 3650
$Comp
L Connector_Generic:Conn_01x01 J1
U 1 1 5C7187C5
P 5350 3050
F 0 "J1" H 5430 3092 50  0000 L CNN
F 1 "Conn_01x01 ANTENNA" H 5430 3001 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 5350 3050 50  0001 C CNN
F 3 "~" H 5350 3050 50  0001 C CNN
	1    5350 3050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5250 3550 5350 3550
Wire Wire Line
	5350 3550 5350 3250
Wire Wire Line
	5250 3750 5650 3750
Wire Wire Line
	5250 3850 5650 3850
$Comp
L power:+5V #PWR0106
U 1 1 5C7161E2
P 3100 1950
F 0 "#PWR0106" H 3100 1800 50  0001 C CNN
F 1 "+5V" H 3115 2123 50  0000 C CNN
F 2 "" H 3100 1950 50  0001 C CNN
F 3 "" H 3100 1950 50  0001 C CNN
	1    3100 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 1950 3100 2150
$Comp
L power:+3.3V #PWR0107
U 1 1 5C716537
P 2900 1950
F 0 "#PWR0107" H 2900 1800 50  0001 C CNN
F 1 "+3.3V" H 2915 2123 50  0000 C CNN
F 2 "" H 2900 1950 50  0001 C CNN
F 3 "" H 2900 1950 50  0001 C CNN
	1    2900 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 1950 2900 2050
Wire Wire Line
	2900 2050 3000 2050
Wire Wire Line
	3000 2050 3000 2150
$Comp
L power:+3.3V #PWR0108
U 1 1 5C716D59
P 3800 3950
F 0 "#PWR0108" H 3800 3800 50  0001 C CNN
F 1 "+3.3V" V 3815 4078 50  0000 L CNN
F 2 "" H 3800 3950 50  0001 C CNN
F 3 "" H 3800 3950 50  0001 C CNN
	1    3800 3950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3800 3950 3900 3950
$Comp
L Device:R_Small R3
U 1 1 5CA666F8
P 5850 3450
F 0 "R3" V 5810 3315 50  0000 C CNN
F 1 "2k7" V 5810 3590 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 5850 3450 50  0001 C CNN
F 3 "~" H 5850 3450 50  0001 C CNN
	1    5850 3450
	0    1    1    0   
$EndComp
Wire Wire Line
	5650 3750 5650 3450
Wire Wire Line
	5650 3450 5750 3450
$Comp
L Device:R_Small R4
U 1 1 5CA6700B
P 5850 4150
F 0 "R4" V 5810 4015 50  0000 C CNN
F 1 "2k7" V 5810 4290 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 5850 4150 50  0001 C CNN
F 3 "~" H 5850 4150 50  0001 C CNN
	1    5850 4150
	0    1    1    0   
$EndComp
Wire Wire Line
	5650 4150 5750 4150
$Comp
L Device:R_Small R5
U 1 1 5CA68AF7
P 6050 3600
F 0 "R5" V 6150 3600 50  0000 C CNN
F 1 "1k" V 5950 3600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 6050 3600 50  0001 C CNN
F 3 "~" H 6050 3600 50  0001 C CNN
	1    6050 3600
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R6
U 1 1 5CA68B61
P 6050 4300
F 0 "R6" V 6150 4300 50  0000 C CNN
F 1 "1k" V 5950 4300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 6050 4300 50  0001 C CNN
F 3 "~" H 6050 4300 50  0001 C CNN
	1    6050 4300
	-1   0    0    1   
$EndComp
Wire Wire Line
	5950 4150 6050 4150
Wire Wire Line
	6050 4150 6050 4200
Wire Wire Line
	5950 3450 6050 3450
Wire Wire Line
	6050 3450 6050 3500
$Comp
L power:GND #PWR0109
U 1 1 5CA69639
P 6050 4500
F 0 "#PWR0109" H 6050 4250 50  0001 C CNN
F 1 "GND" H 6055 4327 50  0000 C CNN
F 2 "" H 6050 4500 50  0001 C CNN
F 3 "" H 6050 4500 50  0001 C CNN
	1    6050 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 4500 6050 4400
$Comp
L power:GND #PWR0110
U 1 1 5CA69BFB
P 6050 3800
F 0 "#PWR0110" H 6050 3550 50  0001 C CNN
F 1 "GND" H 6055 3627 50  0000 C CNN
F 2 "" H 6050 3800 50  0001 C CNN
F 3 "" H 6050 3800 50  0001 C CNN
	1    6050 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 3850 5650 4150
Wire Wire Line
	6050 3700 6050 3800
$Comp
L PAM8403_module:PAM8403_module U3
U 1 1 5CA6C9CB
P 7800 3550
F 0 "U3" H 7800 3565 50  0000 C CNN
F 1 "PAM8403_module" H 7800 3474 50  0000 C CNN
F 2 "PAM8403_module:PAM8403_module" H 7800 3550 50  0001 C CNN
F 3 "" H 7800 3550 50  0001 C CNN
	1    7800 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 3850 6250 3450
Wire Wire Line
	6250 3450 6050 3450
Connection ~ 6050 3450
Wire Wire Line
	6300 3750 6300 4150
Wire Wire Line
	6300 4150 6050 4150
Connection ~ 6050 4150
Wire Wire Line
	6300 3750 6450 3750
Wire Wire Line
	6250 3850 6850 3850
$Comp
L power:GND #PWR0111
U 1 1 5CA6F778
P 7350 3650
F 0 "#PWR0111" H 7350 3400 50  0001 C CNN
F 1 "GND" H 7355 3477 50  0000 C CNN
F 2 "" H 7350 3650 50  0001 C CNN
F 3 "" H 7350 3650 50  0001 C CNN
	1    7350 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	7350 3650 7400 3650
Wire Wire Line
	7400 3650 7400 3800
Wire Wire Line
	7400 3800 7450 3800
$Comp
L power:GND #PWR0112
U 1 1 5CA70055
P 7400 4500
F 0 "#PWR0112" H 7400 4250 50  0001 C CNN
F 1 "GND" H 7405 4327 50  0000 C CNN
F 2 "" H 7400 4500 50  0001 C CNN
F 3 "" H 7400 4500 50  0001 C CNN
	1    7400 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 4500 7400 4050
Wire Wire Line
	7400 4050 7450 4050
$Comp
L power:+5V #PWR0113
U 1 1 5CA7099D
P 6600 4000
F 0 "#PWR0113" H 6600 3850 50  0001 C CNN
F 1 "+5V" H 6615 4173 50  0000 C CNN
F 2 "" H 6600 4000 50  0001 C CNN
F 3 "" H 6600 4000 50  0001 C CNN
	1    6600 4000
	0    -1   -1   0   
$EndComp
$Comp
L Device:L_Small L2
U 1 1 5CA70A18
P 6750 4000
F 0 "L2" V 6850 4100 50  0000 C CNN
F 1 "1u" V 6850 3950 50  0000 C CNN
F 2 "Inductor_THT:L_Axial_L7.0mm_D3.3mm_P10.16mm_Horizontal_Fastron_MICC" H 6750 4000 50  0001 C CNN
F 3 "~" H 6750 4000 50  0001 C CNN
	1    6750 4000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6650 4000 6600 4000
Wire Wire Line
	6850 4000 6900 4000
$Comp
L Device:CP_Small C3
U 1 1 5CA71DAA
P 6900 4300
F 0 "C3" H 6700 4300 50  0000 L CNN
F 1 "47u*" H 6700 4200 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D7.5mm_P2.50mm" H 6900 4300 50  0001 C CNN
F 3 "~" H 6900 4300 50  0001 C CNN
	1    6900 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4200 6900 4000
Connection ~ 6900 4000
Wire Wire Line
	6900 4000 7200 4000
$Comp
L power:GND #PWR0114
U 1 1 5CA7284C
P 6900 4500
F 0 "#PWR0114" H 6900 4250 50  0001 C CNN
F 1 "GND" H 6905 4327 50  0000 C CNN
F 2 "" H 6900 4500 50  0001 C CNN
F 3 "" H 6900 4500 50  0001 C CNN
	1    6900 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4400 6900 4500
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 5CA738C7
P 9600 3450
F 0 "J2" H 9680 3442 50  0000 L CNN
F 1 "Conn_01x04" H 9680 3351 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9600 3450 50  0001 C CNN
F 3 "~" H 9600 3450 50  0001 C CNN
	1    9600 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:Ferrite_Bead FB1
U 1 1 5CA7F6C5
P 8500 2550
F 0 "FB1" V 8318 2550 50  0000 C CNN
F 1 "Ferrite_Bead" V 8317 2550 50  0001 C CNN
F 2 "Inductor_THT:L_Axial_L9.5mm_D4.0mm_P12.70mm_Horizontal_Fastron_SMCC" V 8430 2550 50  0001 C CNN
F 3 "~" H 8500 2550 50  0001 C CNN
	1    8500 2550
	0    1    1    0   
$EndComp
$Comp
L Device:Ferrite_Bead FB2
U 1 1 5CA80393
P 8500 3050
F 0 "FB2" V 8318 3050 50  0000 C CNN
F 1 "Ferrite_Bead" V 8317 3050 50  0001 C CNN
F 2 "Inductor_THT:L_Axial_L9.5mm_D4.0mm_P12.70mm_Horizontal_Fastron_SMCC" V 8430 3050 50  0001 C CNN
F 3 "~" H 8500 3050 50  0001 C CNN
	1    8500 3050
	0    1    1    0   
$EndComp
$Comp
L Device:Ferrite_Bead FB3
U 1 1 5CA81B08
P 8500 3550
F 0 "FB3" V 8318 3550 50  0000 C CNN
F 1 "Ferrite_Bead" V 8317 3550 50  0001 C CNN
F 2 "Inductor_THT:L_Axial_L9.5mm_D4.0mm_P12.70mm_Horizontal_Fastron_SMCC" V 8430 3550 50  0001 C CNN
F 3 "~" H 8500 3550 50  0001 C CNN
	1    8500 3550
	0    1    1    0   
$EndComp
$Comp
L Device:Ferrite_Bead FB4
U 1 1 5CA81B6E
P 8500 4050
F 0 "FB4" V 8318 4050 50  0000 C CNN
F 1 "Ferrite_Bead" V 8317 4050 50  0001 C CNN
F 2 "Inductor_THT:L_Axial_L9.5mm_D4.0mm_P12.70mm_Horizontal_Fastron_SMCC" V 8430 4050 50  0001 C CNN
F 3 "~" H 8500 4050 50  0001 C CNN
	1    8500 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	8150 4050 8350 4050
Wire Wire Line
	8150 4000 8350 4000
Wire Wire Line
	8150 3800 8300 3800
Wire Wire Line
	8150 3750 8250 3750
$Comp
L Device:C_Small C5
U 1 1 5CA8A592
P 8750 2650
F 0 "C5" H 8850 2650 50  0000 L CNN
F 1 "220p" H 8850 2550 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 8750 2650 50  0001 C CNN
F 3 "~" H 8750 2650 50  0001 C CNN
	1    8750 2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 5CA8A74A
P 8750 2800
F 0 "#PWR0115" H 8750 2550 50  0001 C CNN
F 1 "GND" H 8755 2627 50  0000 C CNN
F 2 "" H 8750 2800 50  0001 C CNN
F 3 "" H 8750 2800 50  0001 C CNN
	1    8750 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 3550 8350 4000
Wire Wire Line
	8300 3800 8300 3050
Wire Wire Line
	8300 3050 8350 3050
Wire Wire Line
	8250 3750 8250 2550
Wire Wire Line
	8250 2550 8350 2550
Wire Wire Line
	8650 2550 8750 2550
Wire Wire Line
	8750 2750 8750 2800
$Comp
L Device:C_Small C6
U 1 1 5CA92783
P 8750 3150
F 0 "C6" H 8850 3150 50  0000 L CNN
F 1 "220p" H 8850 3050 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 8750 3150 50  0001 C CNN
F 3 "~" H 8750 3150 50  0001 C CNN
	1    8750 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 5CA92789
P 8750 3300
F 0 "#PWR0116" H 8750 3050 50  0001 C CNN
F 1 "GND" H 8755 3127 50  0000 C CNN
F 2 "" H 8750 3300 50  0001 C CNN
F 3 "" H 8750 3300 50  0001 C CNN
	1    8750 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 3050 8750 3050
Wire Wire Line
	8750 3250 8750 3300
$Comp
L Device:C_Small C7
U 1 1 5CA93786
P 8750 3650
F 0 "C7" H 8850 3650 50  0000 L CNN
F 1 "220p" H 8850 3550 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 8750 3650 50  0001 C CNN
F 3 "~" H 8750 3650 50  0001 C CNN
	1    8750 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 5CA9378C
P 8750 3800
F 0 "#PWR0117" H 8750 3550 50  0001 C CNN
F 1 "GND" H 8755 3627 50  0000 C CNN
F 2 "" H 8750 3800 50  0001 C CNN
F 3 "" H 8750 3800 50  0001 C CNN
	1    8750 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 3550 8750 3550
Wire Wire Line
	8750 3750 8750 3800
$Comp
L Device:C_Small C8
U 1 1 5CA9487F
P 8750 4150
F 0 "C8" H 8850 4150 50  0000 L CNN
F 1 "220p" H 8850 4050 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 8750 4150 50  0001 C CNN
F 3 "~" H 8750 4150 50  0001 C CNN
	1    8750 4150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5CA94885
P 8750 4300
F 0 "#PWR0118" H 8750 4050 50  0001 C CNN
F 1 "GND" H 8755 4127 50  0000 C CNN
F 2 "" H 8750 4300 50  0001 C CNN
F 3 "" H 8750 4300 50  0001 C CNN
	1    8750 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 4050 8750 4050
Wire Wire Line
	8750 4250 8750 4300
Wire Wire Line
	8750 3050 9400 3050
Connection ~ 8750 3050
Wire Wire Line
	8750 2550 9300 2550
Connection ~ 8750 2550
Wire Wire Line
	9400 3050 9400 3350
Wire Wire Line
	9300 2550 9300 3450
Wire Wire Line
	9300 3450 9400 3450
Wire Wire Line
	8750 3550 9400 3550
Connection ~ 8750 3550
Wire Wire Line
	8750 4050 9300 4050
Wire Wire Line
	9300 4050 9300 3650
Wire Wire Line
	9300 3650 9400 3650
Connection ~ 8750 4050
$Comp
L Device:CP_Small C4
U 1 1 5CAA5E9E
P 7200 4300
F 0 "C4" H 7000 4300 50  0000 L CNN
F 1 "47u" H 7000 4200 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D7.5mm_P2.50mm" H 7200 4300 50  0001 C CNN
F 3 "~" H 7200 4300 50  0001 C CNN
	1    7200 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 5CAA5EA4
P 7200 4500
F 0 "#PWR0119" H 7200 4250 50  0001 C CNN
F 1 "GND" H 7205 4327 50  0000 C CNN
F 2 "" H 7200 4500 50  0001 C CNN
F 3 "" H 7200 4500 50  0001 C CNN
	1    7200 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 4400 7200 4500
Wire Wire Line
	7200 4200 7200 4000
Connection ~ 7200 4000
Wire Wire Line
	7200 4000 7450 4000
$Comp
L Device:R_Small R7
U 1 1 5CAA9477
P 1950 3850
F 0 "R7" V 1850 3850 50  0000 C CNN
F 1 "2k7" V 2050 3850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 1950 3850 50  0001 C CNN
F 3 "~" H 1950 3850 50  0001 C CNN
	1    1950 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	2050 3850 2400 3850
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 5CAAAF57
P 1400 3950
F 0 "J3" H 1320 3625 50  0000 C CNN
F 1 "Conn_01x02" H 1320 3716 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1400 3950 50  0001 C CNN
F 3 "~" H 1400 3950 50  0001 C CNN
	1    1400 3950
	-1   0    0    1   
$EndComp
Wire Wire Line
	1600 3850 1850 3850
$Comp
L power:GND #PWR0120
U 1 1 5CAACA30
P 1700 4250
F 0 "#PWR0120" H 1700 4000 50  0001 C CNN
F 1 "GND" H 1705 4077 50  0000 C CNN
F 2 "" H 1700 4250 50  0001 C CNN
F 3 "" H 1700 4250 50  0001 C CNN
	1    1700 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 4250 1700 3950
Wire Wire Line
	1700 3950 1600 3950
Text Notes 1300 4150 1    50   ~ 0
Extra LED
Text Notes 5450 2950 1    50   ~ 0
ANTENNA
Text Notes 9700 3700 0    50   ~ 0
SPEAKERS
$Comp
L Device:C_Small C9
U 1 1 5CAAF0ED
P 6450 2650
F 0 "C9" H 6550 2650 50  0000 L CNN
F 1 "220n" H 6550 2550 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.2mm_W3.5mm_P5.00mm_FKS2_FKP2_MKS2_MKP2" H 6450 2650 50  0001 C CNN
F 3 "~" H 6450 2650 50  0001 C CNN
	1    6450 2650
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C10
U 1 1 5CAAF1BE
P 6850 2650
F 0 "C10" H 6950 2650 50  0000 L CNN
F 1 "220n" H 6950 2550 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.2mm_W3.5mm_P5.00mm_FKS2_FKP2_MKS2_MKP2" H 6850 2650 50  0001 C CNN
F 3 "~" H 6850 2650 50  0001 C CNN
	1    6850 2650
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J4
U 1 1 5CAB2CCE
P 1650 1800
F 0 "J4" H 1900 1450 50  0000 C CNN
F 1 "Conn_01x03" H 1700 1550 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 1650 1800 50  0001 C CNN
F 3 "~" H 1650 1800 50  0001 C CNN
	1    1650 1800
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0121
U 1 1 5CAB2E59
P 1850 1950
F 0 "#PWR0121" H 1850 1700 50  0001 C CNN
F 1 "GND" H 1855 1777 50  0000 C CNN
F 2 "" H 1850 1950 50  0001 C CNN
F 3 "" H 1850 1950 50  0001 C CNN
	1    1850 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 1950 1850 1900
$Comp
L power:+5V #PWR0122
U 1 1 5CAB4C93
P 1850 1450
F 0 "#PWR0122" H 1850 1300 50  0001 C CNN
F 1 "+5V" H 1865 1623 50  0000 C CNN
F 2 "" H 1850 1450 50  0001 C CNN
F 3 "" H 1850 1450 50  0001 C CNN
	1    1850 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 1450 1850 1700
Wire Wire Line
	1850 1800 2000 1800
Wire Wire Line
	2000 1800 2000 2750
Connection ~ 2000 2750
Wire Wire Line
	2000 2750 2400 2750
Text Notes 1550 2000 1    50   ~ 0
IR sensor
$Comp
L Connector_Generic:Conn_01x03 J5
U 1 1 5CAB90CA
P 6450 2250
F 0 "J5" H 6700 1900 50  0000 C CNN
F 1 "Conn_01x03" H 6500 2000 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6450 2250 50  0001 C CNN
F 3 "~" H 6450 2250 50  0001 C CNN
	1    6450 2250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R8
U 1 1 5CABD851
P 6450 3000
F 0 "R8" V 6550 3000 50  0000 C CNN
F 1 "4k7" V 6350 3000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 6450 3000 50  0001 C CNN
F 3 "~" H 6450 3000 50  0001 C CNN
	1    6450 3000
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R9
U 1 1 5CABD95E
P 6850 3000
F 0 "R9" V 6950 3000 50  0000 C CNN
F 1 "4k7" V 6750 3000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" H 6850 3000 50  0001 C CNN
F 3 "~" H 6850 3000 50  0001 C CNN
	1    6850 3000
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 5CABD9FE
P 6200 2600
F 0 "#PWR0123" H 6200 2350 50  0001 C CNN
F 1 "GND" H 6205 2427 50  0000 C CNN
F 2 "" H 6200 2600 50  0001 C CNN
F 3 "" H 6200 2600 50  0001 C CNN
	1    6200 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 2600 6200 2550
Wire Wire Line
	6200 2550 6350 2550
Wire Wire Line
	6350 2550 6350 2450
Wire Wire Line
	6450 2550 6450 2450
Wire Wire Line
	6550 2450 6550 2550
Wire Wire Line
	6550 2550 6850 2550
Wire Wire Line
	6450 2750 6450 2900
Wire Wire Line
	6850 2750 6850 2900
Wire Wire Line
	6450 3100 6450 3750
Connection ~ 6450 3750
Wire Wire Line
	6450 3750 7450 3750
Wire Wire Line
	6850 3100 6850 3850
Connection ~ 6850 3850
Wire Wire Line
	6850 3850 7450 3850
Text Notes 6300 2150 0    50   ~ 0
AUX IN
Text Notes 7400 7500 0    87   ~ 0
FM radio  http://tomeko.net/projects/RDA5807M_radio/
$Comp
L Mechanical:MountingHole H1
U 1 1 5CAD0C71
P 9900 5750
F 0 "H1" H 10000 5796 50  0000 L CNN
F 1 "MountingHole" H 10000 5705 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9900 5750 50  0001 C CNN
F 3 "~" H 9900 5750 50  0001 C CNN
	1    9900 5750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5CAD974E
P 9900 5950
F 0 "H2" H 10000 5996 50  0000 L CNN
F 1 "MountingHole" H 10000 5905 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9900 5950 50  0001 C CNN
F 3 "~" H 9900 5950 50  0001 C CNN
	1    9900 5950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5CAD97DA
P 9900 6150
F 0 "H3" H 10000 6196 50  0000 L CNN
F 1 "MountingHole" H 10000 6105 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9900 6150 50  0001 C CNN
F 3 "~" H 9900 6150 50  0001 C CNN
	1    9900 6150
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 5CAD983C
P 9900 6350
F 0 "H4" H 10000 6396 50  0000 L CNN
F 1 "MountingHole" H 10000 6305 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9900 6350 50  0001 C CNN
F 3 "~" H 9900 6350 50  0001 C CNN
	1    9900 6350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
