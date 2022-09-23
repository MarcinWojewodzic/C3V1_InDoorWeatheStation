EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
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
L STM32L4Q5CGT6:STM32L4Q5CGT6 IC1
U 1 1 62DD139C
P 6000 5900
F 0 "IC1" H 7744 5496 50  0000 L CNN
F 1 "STM32L4Q5CGT6" H 7744 5405 50  0000 L CNN
F 2 "Projekt:QFP50P900X900X160-48N" H 7550 6600 50  0001 L CNN
F 3 "https://www.st.com/resource/en/datasheet/stm32l4q5ag.pdf" H 7550 6500 50  0001 L CNN
F 4 "Ultra-low-power with FPU Arm Cortex-M4 MCU 120 MHz with 1024 kbytes of Flash memory, USB OTG, DFSDM, CHROM-ART, AES, PKA" H 7550 6400 50  0001 L CNN "Description"
F 5 "1.6" H 7550 6300 50  0001 L CNN "Height"
F 6 "STMicroelectronics" H 7550 6200 50  0001 L CNN "Manufacturer_Name"
F 7 "STM32L4Q5CGT6" H 7550 6100 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "511-STM32L4Q5CGT6" H 7550 6000 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.co.uk/ProductDetail/STMicroelectronics/STM32L4Q5CGT6?qs=xZ%2FP%252Ba9zWqa0QI8asRSMRw%3D%3D" H 7550 5900 50  0001 L CNN "Mouser Price/Stock"
F 10 "" H 7550 5800 50  0001 L CNN "Arrow Part Number"
F 11 "" H 7550 5700 50  0001 L CNN "Arrow Price/Stock"
	1    6000 5900
	1    0    0    -1  
$EndComp
Text GLabel 6000 6700 0    50   Input ~ 0
VCC_3V3
Text GLabel 6300 5000 1    50   Input ~ 0
VCC_3V3
Text GLabel 6000 6600 0    50   Input ~ 0
GND
Text GLabel 6400 5000 1    50   Input ~ 0
GND
$Comp
L Connector:Conn_01x06_Male J1
U 1 1 62DD54DA
P 950 650
F 0 "J1" V 1012 894 50  0000 L CNN
F 1 "FLASH" V 1103 894 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 950 650 50  0001 C CNN
F 3 "~" H 950 650 50  0001 C CNN
	1    950  650 
	0    1    1    0   
$EndComp
Text GLabel 650  850  3    50   Input ~ 0
VCC_3V3
Text GLabel 950  850  3    50   Input ~ 0
GND
Text GLabel 750  850  3    50   Input ~ 0
FLASH_CS
Text GLabel 850  850  3    50   Input ~ 0
MEMORY_MISO
Text GLabel 1050 850  3    50   Input ~ 0
MEMORY_SCK
Text GLabel 1150 850  3    50   Input ~ 0
MEMORY_MOSI
Wire Notes Line
	1500 1500 1500 450 
Text Notes 450  1500 0    50   ~ 0
FLASH MEMORY
Text GLabel 1600 700  1    50   Input ~ 0
GND
Wire Wire Line
	1600 700  1600 800 
$Comp
L power:GND #PWR0101
U 1 1 62DD7623
P 1600 800
F 0 "#PWR0101" H 1600 550 50  0001 C CNN
F 1 "GND" H 1605 627 50  0000 C CNN
F 2 "" H 1600 800 50  0001 C CNN
F 3 "" H 1600 800 50  0001 C CNN
	1    1600 800 
	1    0    0    -1  
$EndComp
Text Notes 1500 1500 0    50   ~ 0
GND
Wire Notes Line
	1750 1500 1750 450 
$Comp
L Connector:Conn_01x08_Male J2
U 1 1 62DD7E4E
P 2250 650
F 0 "J2" V 2085 578 50  0000 C CNN
F 1 "FRAM_MEMORY" V 2176 578 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 2250 650 50  0001 C CNN
F 3 "~" H 2250 650 50  0001 C CNN
	1    2250 650 
	0    1    1    0   
$EndComp
Text GLabel 2250 850  3    50   Input ~ 0
MEMORY_MISO
Text GLabel 2150 850  3    50   Input ~ 0
MEMORY_SCK
Text GLabel 2350 850  3    50   Input ~ 0
MEMORY_MOSI
Text GLabel 1850 850  3    50   Input ~ 0
VCC_3V3
Text GLabel 1950 850  3    50   Input ~ 0
GND
Text GLabel 2050 850  3    50   Input ~ 0
FRAM_HOLD
Text GLabel 2450 850  3    50   Input ~ 0
FRAM_CS
Text GLabel 2550 850  3    50   Input ~ 0
FRAM_WP
Wire Notes Line
	2650 1500 2650 450 
Text Notes 1750 1500 0    50   ~ 0
FRAM_MEMORY
Text GLabel 6500 5000 1    50   Input ~ 0
FLASH_CS
Text GLabel 7100 5000 1    50   Input ~ 0
MEMORY_MISO
Text GLabel 7200 5000 1    50   Input ~ 0
MEMORY_SCK
Text GLabel 7000 5000 1    50   Input ~ 0
MEMORY_MOSI
Text GLabel 6900 5000 1    50   Input ~ 0
FRAM_CS
Text GLabel 6800 5000 1    50   Input ~ 0
FRAM_WP
Text GLabel 6600 5000 1    50   Input ~ 0
FRAM_HOLD
$Comp
L Connector:Conn_01x08_Male J4
U 1 1 62DDB013
P 3150 650
F 0 "J4" V 2985 578 50  0000 C CNN
F 1 "PMS3003" V 3076 578 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 3150 650 50  0001 C CNN
F 3 "~" H 3150 650 50  0001 C CNN
	1    3150 650 
	0    1    1    0   
$EndComp
Text GLabel 3450 850  3    50   Input ~ 0
VCC_5V
Text GLabel 3350 850  3    50   Input ~ 0
GND
Text GLabel 2950 850  3    50   Input ~ 0
PMS3003_SET
Text GLabel 3150 850  3    50   Input ~ 0
PMS3003_RX
Text GLabel 3050 850  3    50   Input ~ 0
PMS3003_TX
Text GLabel 3250 850  3    50   Input ~ 0
PMS3003_RST
Text GLabel 2750 850  3    50   Input ~ 0
GND
Text GLabel 2850 850  3    50   Input ~ 0
GND
Wire Notes Line
	3650 1500 3650 450 
Text Notes 2650 1500 0    50   ~ 0
PMS3003
Text GLabel 7700 6200 2    50   Input ~ 0
PMS3003_SET
Text GLabel 6300 7700 3    50   Input ~ 0
PMS3003_TX
Text GLabel 6000 6000 0    50   Input ~ 0
PMS3003_RST
Text GLabel 6000 7000 0    50   Input ~ 0
PMS3003_RX
Text GLabel 7700 5900 2    50   Input ~ 0
VCC_3V3
Text GLabel 7700 6000 2    50   Input ~ 0
GND
Text GLabel 7400 7700 3    50   Input ~ 0
VCC_3V3
Text GLabel 7300 7700 3    50   Input ~ 0
GND
Text GLabel 3800 850  3    50   Input ~ 0
GND
Text GLabel 3900 850  3    50   Input ~ 0
VIN
Text Notes 3650 1500 0    50   ~ 0
DC IN
$Comp
L LD1117V33:LD1117V33 IC2
U 1 1 62DE1B2A
P 6300 800
F 0 "IC2" H 7228 746 50  0000 L CNN
F 1 "LD1117V33" H 7228 655 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 7250 900 50  0001 L CNN
F 3 "http://www.st.com/web/en/resource/technical/document/datasheet/CD00000544.pdf" H 7250 800 50  0001 L CNN
F 4 "800mA,3.3V,LDO Voltage Reg.,LD1117V33 STMicroelectronics LD1117V33, LDO Voltage Regulator, 1.3A, 3.3 V +/-1%, 3-Pin TO-220" H 7250 700 50  0001 L CNN "Description"
F 5 "4.6" H 7250 600 50  0001 L CNN "Height"
F 6 "STMicroelectronics" H 7250 500 50  0001 L CNN "Manufacturer_Name"
F 7 "LD1117V33" H 7250 400 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "511-LD1117V33" H 7250 300 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.co.uk/ProductDetail/STMicroelectronics/LD1117V33?qs=arR7071FstdSYX%2FAthFGRA%3D%3D" H 7250 200 50  0001 L CNN "Mouser Price/Stock"
F 10 "LD1117V33" H 7250 100 50  0001 L CNN "Arrow Part Number"
F 11 "https://www.arrow.com/en/products/ld1117v33/stmicroelectronics?region=europe" H 7250 0   50  0001 L CNN "Arrow Price/Stock"
	1    6300 800 
	1    0    0    -1  
$EndComp
Text GLabel 5800 800  0    50   Input ~ 0
GND
Text GLabel 4450 850  0    50   Input ~ 0
VIN
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J6
U 1 1 62DE3B43
P 3900 650
F 0 "J6" H 3950 867 50  0000 C CNN
F 1 "DC_IN" H 3950 776 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 3900 650 50  0001 C CNN
F 3 "~" H 3900 650 50  0001 C CNN
	1    3900 650 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 650  4200 750 
Wire Wire Line
	3700 650  3700 750 
Wire Wire Line
	3700 750  3700 850 
Wire Wire Line
	3700 850  3800 850 
Connection ~ 3700 750 
Wire Wire Line
	4200 750  4200 850 
Wire Wire Line
	4200 850  3900 850 
Connection ~ 4200 750 
Text GLabel 4850 1250 3    50   Input ~ 0
GND
Text GLabel 5250 850  2    50   Input ~ 0
VCC_5V
$Comp
L Device:C C2
U 1 1 62DE4C00
P 5200 1100
F 0 "C2" H 5315 1146 50  0000 L CNN
F 1 "100n" H 5315 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 5238 950 50  0001 C CNN
F 3 "~" H 5200 1100 50  0001 C CNN
	1    5200 1100
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:L7805 U2
U 1 1 62DDEE86
P 4850 850
F 0 "U2" H 4850 1092 50  0000 C CNN
F 1 "L7805" H 4850 1001 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4875 700 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 4850 800 50  0001 C CNN
	1    4850 850 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 62DE6B2A
P 4500 1100
F 0 "C1" H 4615 1146 50  0000 L CNN
F 1 "100n" H 4615 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 4538 950 50  0001 C CNN
F 3 "~" H 4500 1100 50  0001 C CNN
	1    4500 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 850  5200 850 
Wire Wire Line
	5200 850  5200 950 
Connection ~ 5200 850 
Wire Wire Line
	5200 850  5250 850 
Wire Wire Line
	5200 1250 4850 1250
Wire Wire Line
	4850 1250 4850 1150
Wire Wire Line
	4850 1250 4500 1250
Connection ~ 4850 1250
Wire Wire Line
	4500 950  4500 850 
Wire Wire Line
	4500 850  4550 850 
Wire Wire Line
	4500 850  4450 850 
Connection ~ 4500 850 
$Comp
L Device:C C3
U 1 1 62DE82EC
P 5900 950
F 0 "C3" H 6015 996 50  0000 L CNN
F 1 "100n" H 6015 905 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 5938 800 50  0001 C CNN
F 3 "~" H 5900 950 50  0001 C CNN
	1    5900 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 62DE8507
P 6100 950
F 0 "C4" H 6215 996 50  0000 L CNN
F 1 "100n" H 6215 905 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 6138 800 50  0001 C CNN
F 3 "~" H 6100 950 50  0001 C CNN
	1    6100 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 800  6100 800 
Wire Wire Line
	6100 800  5900 800 
Connection ~ 6100 800 
Wire Wire Line
	5900 800  5800 800 
Connection ~ 5900 800 
Wire Wire Line
	6300 900  6000 900 
Wire Wire Line
	6000 900  6000 1100
Wire Wire Line
	6000 1100 5900 1100
Wire Wire Line
	6300 1000 6200 1000
Wire Wire Line
	6200 1000 6200 1100
Wire Wire Line
	6200 1100 6100 1100
Text GLabel 6150 1100 3    50   Input ~ 0
VCC_5V
Text GLabel 6000 1100 3    50   Input ~ 0
VCC_3V3
Wire Notes Line
	7750 1500 7750 450 
Text Notes 4250 1500 0    50   ~ 0
STABILIZATORY
Text GLabel 7950 650  1    50   Input ~ 0
VIN
$Comp
L Device:CP C5
U 1 1 62DEB375
P 7950 800
F 0 "C5" H 8068 846 50  0000 L CNN
F 1 "470u" H 8068 755 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_8x10.5" H 7988 650 50  0001 C CNN
F 3 "~" H 7950 800 50  0001 C CNN
	1    7950 800 
	1    0    0    -1  
$EndComp
Text GLabel 7950 950  3    50   Input ~ 0
GND
Text GLabel 9200 850  1    50   Input ~ 0
VCC_3V3
$Comp
L Device:CP C11
U 1 1 62DEC249
P 9750 1050
F 0 "C11" H 9868 1096 50  0000 L CNN
F 1 "10u" H 9868 1005 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 9788 900 50  0001 C CNN
F 3 "~" H 9750 1050 50  0001 C CNN
	1    9750 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C10
U 1 1 62DECC27
P 9450 1050
F 0 "C10" H 9568 1096 50  0000 L CNN
F 1 "100u" H 9568 1005 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_5x5.4" H 9488 900 50  0001 C CNN
F 3 "~" H 9450 1050 50  0001 C CNN
	1    9450 1050
	1    0    0    -1  
$EndComp
Text GLabel 9200 1200 3    50   Input ~ 0
GND
Wire Wire Line
	9200 850  9200 900 
Wire Wire Line
	9750 900  9450 900 
Wire Wire Line
	9450 900  9200 900 
Connection ~ 9450 900 
Connection ~ 9200 900 
Wire Wire Line
	9450 1200 9750 1200
Text GLabel 10850 800  1    50   Input ~ 0
VCC_5V
$Comp
L Device:CP C14
U 1 1 62DF2046
P 11050 1000
F 0 "C14" H 11168 1046 50  0000 L CNN
F 1 "100u" H 11168 955 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_5x5.4" H 11088 850 50  0001 C CNN
F 3 "~" H 11050 1000 50  0001 C CNN
	1    11050 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 850  10850 850 
Wire Wire Line
	11050 1150 10650 1150
Wire Wire Line
	10850 800  10850 850 
Connection ~ 10850 850 
Wire Wire Line
	10850 850  11050 850 
Text GLabel 10850 1150 3    50   Input ~ 0
GND
Wire Notes Line
	11400 1500 11400 450 
Text Notes 7800 1450 0    50   ~ 0
CAPACITORS
$Comp
L Connector:Conn_01x05_Male J10
U 1 1 62DFA41B
P 11700 600
F 0 "J10" V 11762 844 50  0000 L CNN
F 1 "HC12" V 11853 844 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 11700 600 50  0001 C CNN
F 3 "~" H 11700 600 50  0001 C CNN
	1    11700 600 
	0    1    1    0   
$EndComp
Text GLabel 11900 800  3    50   Input ~ 0
VCC_5V
Text GLabel 11800 800  3    50   Input ~ 0
GND
Text GLabel 11700 800  3    50   Input ~ 0
HC12_RX
Text GLabel 11600 800  3    50   Input ~ 0
HC12_TX
Text GLabel 11500 800  3    50   Input ~ 0
HC12_SET
Wire Notes Line
	12200 1500 12200 450 
Text GLabel 7700 6500 2    50   Input ~ 0
HC12_RX
Text GLabel 7700 6400 2    50   Input ~ 0
HC12_TX
Text GLabel 7700 6300 2    50   Input ~ 0
HC12_SET
$Comp
L Connector:Conn_01x03_Male J11
U 1 1 62DFFE82
P 12950 600
F 0 "J11" V 13012 744 50  0000 L CNN
F 1 "WS2812" V 13103 744 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 12950 600 50  0001 C CNN
F 3 "~" H 12950 600 50  0001 C CNN
	1    12950 600 
	0    1    1    0   
$EndComp
Text GLabel 12950 800  3    50   Input ~ 0
VCC_5V
Text GLabel 12850 800  3    50   Input ~ 0
GND
Text GLabel 13050 1100 3    50   Input ~ 0
WS_TIM
Text GLabel 7700 6600 2    50   Input ~ 0
WS_TIM
Wire Notes Line
	13650 1500 13650 450 
Text GLabel 12500 800  1    50   Input ~ 0
VCC_5V
Wire Wire Line
	12500 800  12500 850 
Text GLabel 12500 1150 3    50   Input ~ 0
GND
Text Notes 12200 1500 0    50   ~ 0
WS2812
$Comp
L Device:R R4
U 1 1 62E0EBB9
P 13050 950
F 0 "R4" H 13120 996 50  0000 L CNN
F 1 "1k" H 13120 905 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 12980 950 50  0001 C CNN
F 3 "~" H 13050 950 50  0001 C CNN
	1    13050 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 62E0FBF8
P 13150 950
F 0 "R5" H 13220 996 50  0000 L CNN
F 1 "1k" H 13220 905 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 13080 950 50  0001 C CNN
F 3 "~" H 13150 950 50  0001 C CNN
	1    13150 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	13150 800  13050 800 
Connection ~ 13050 800 
Wire Wire Line
	13150 1100 13050 1100
$Comp
L Device:LED D2
U 1 1 62E1198F
P 14600 850
F 0 "D2" H 14593 595 50  0000 C CNN
F 1 "GREEN" H 14593 686 50  0000 C CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 14600 850 50  0001 C CNN
F 3 "~" H 14600 850 50  0001 C CNN
	1    14600 850 
	-1   0    0    1   
$EndComp
$Comp
L Device:R R6
U 1 1 62E132C3
P 14300 850
F 0 "R6" V 14507 850 50  0000 C CNN
F 1 "360" V 14416 850 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 14230 850 50  0001 C CNN
F 3 "~" H 14300 850 50  0001 C CNN
	1    14300 850 
	0    -1   -1   0   
$EndComp
Text GLabel 14850 950  3    50   Input ~ 0
GND
Wire Wire Line
	14750 850  14850 850 
Wire Wire Line
	14850 850  14850 950 
Text GLabel 14150 850  0    50   Input ~ 0
HEART_BIT
Wire Notes Line
	450  1500 16100 1500
Text Notes 13800 1500 0    50   ~ 0
HEART_BIT
Text GLabel 7700 7000 2    50   Input ~ 0
HEART_BIT
$Comp
L Sensor_Temperature:DS18B20 U1
U 1 1 62E192A7
P 1100 2200
F 0 "U1" H 870 2246 50  0000 R CNN
F 1 "DS18B20" H 870 2155 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 100 1950 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 950 2450 50  0001 C CNN
	1    1100 2200
	1    0    0    -1  
$EndComp
Text GLabel 1100 1900 1    50   Input ~ 0
VCC_3V3
$Comp
L Device:R R1
U 1 1 62E1C0F9
P 1400 1900
F 0 "R1" V 1607 1900 50  0000 C CNN
F 1 "4,7k" V 1516 1900 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 1330 1900 50  0001 C CNN
F 3 "~" H 1400 1900 50  0001 C CNN
	1    1400 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1250 1900 1100 1900
Wire Wire Line
	1550 1900 1550 2200
Wire Wire Line
	1550 2200 1400 2200
Text GLabel 1100 2500 3    50   Input ~ 0
GND
Text GLabel 1550 2200 2    50   Input ~ 0
DS18B20
Wire Notes Line
	2050 2800 2050 1500
Text Notes 500  2800 0    50   ~ 0
DS18B20
Text GLabel 7700 6700 2    50   Input ~ 0
DS18B20
$Comp
L Connector:Conn_01x05_Male J3
U 1 1 62E21E4E
P 2400 1700
F 0 "J3" V 2462 1944 50  0000 L CNN
F 1 "ENCODER" V 2553 1944 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 2400 1700 50  0001 C CNN
F 3 "~" H 2400 1700 50  0001 C CNN
	1    2400 1700
	0    1    1    0   
$EndComp
Text GLabel 3750 1900 3    50   Input ~ 0
VCC_3V3
Text GLabel 3650 1900 3    50   Input ~ 0
GND
Text GLabel 2400 1900 3    50   Input ~ 0
ENCODER_SW
Text GLabel 2500 1900 3    50   Input ~ 0
ENCODER_SIA
Text GLabel 2600 1900 3    50   Input ~ 0
ENCODER_SIB
Wire Notes Line
	3100 2800 3100 1500
Text Notes 2050 2800 0    50   ~ 0
ENCODER
$Comp
L Connector:Conn_01x06_Male J5
U 1 1 62E26D13
P 3550 1700
F 0 "J5" V 3612 1944 50  0000 L CNN
F 1 "BME280" V 3703 1944 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 3550 1700 50  0001 C CNN
F 3 "~" H 3550 1700 50  0001 C CNN
	1    3550 1700
	0    1    1    0   
$EndComp
Text GLabel 3250 1900 3    50   Input ~ 0
SCREEN_BME_MISO
Text GLabel 3350 1900 3    50   Input ~ 0
BME280_CS
Text GLabel 3450 1900 3    50   Input ~ 0
SCREEN_BME_MOSI
Text GLabel 3550 1900 3    50   Input ~ 0
SCREEN_BME_SCK
Wire Notes Line
	4250 450  4250 2800
Text Notes 3100 2800 0    50   ~ 0
BME280
Text GLabel 2200 1900 3    50   Input ~ 0
VCC_3V3
Text GLabel 2300 1900 3    50   Input ~ 0
GND
Text GLabel 4800 1900 3    50   Input ~ 0
GND
Text GLabel 6100 1900 3    50   Input ~ 0
SCREEN_BME_MOSI
Text GLabel 6200 1900 3    50   Input ~ 0
SCREEN_BME_SCK
Text GLabel 5000 1900 3    50   Input ~ 0
SSD1331_CS
Text GLabel 4500 1900 3    50   Input ~ 0
EPAPIER_SSD1331_DC
Text GLabel 4400 1900 3    50   Input ~ 0
SSD1331_RST
Wire Notes Line
	5550 2800 5550 1500
Text Notes 4600 2800 0    50   ~ 0
SSD1331
$Comp
L Connector:Conn_01x08_Male J8
U 1 1 62E37573
P 6100 1700
F 0 "J8" V 5935 1628 50  0000 C CNN
F 1 "EPAPIER" V 6026 1628 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 6100 1700 50  0001 C CNN
F 3 "~" H 6100 1700 50  0001 C CNN
	1    6100 1700
	0    1    1    0   
$EndComp
Text GLabel 6300 1900 3    50   Input ~ 0
GND
Text GLabel 5900 1900 3    50   Input ~ 0
EPAPIER_CS
Text GLabel 6000 1900 3    50   Input ~ 0
EPAPIER_SSD1331_DC
Text GLabel 5700 1900 3    50   Input ~ 0
EPAPIER_RST
Text GLabel 5800 1900 3    50   Input ~ 0
EPAPIER_BUSY
Wire Notes Line
	6700 2800 6700 1500
Text Notes 6350 2800 0    50   ~ 0
EPAPIER
Text GLabel 7700 6800 2    50   Input ~ 0
ENCODER_SW
Text GLabel 6000 6800 0    50   Input ~ 0
ENCODER_SIA
Text GLabel 6000 6900 0    50   Input ~ 0
ENCODER_SIB
Text GLabel 6600 7700 3    50   Input ~ 0
SCREEN_BME_MISO
Text GLabel 7700 6900 2    50   Input ~ 0
BME280_CS
Text GLabel 6700 7700 3    50   Input ~ 0
SCREEN_BME_MOSI
Text GLabel 6500 7700 3    50   Input ~ 0
SCREEN_BME_SCK
Text GLabel 4600 1900 3    50   Input ~ 0
SCREEN_BME_MOSI
Text GLabel 4700 1900 3    50   Input ~ 0
SCREEN_BME_SCK
Text GLabel 6400 7700 3    50   Input ~ 0
SSD1331_CS
Text GLabel 6800 7700 3    50   Input ~ 0
EPAPIER_SSD1331_DC
Text GLabel 7200 7700 3    50   Input ~ 0
SSD1331_RST
Text GLabel 6900 7700 3    50   Input ~ 0
EPAPIER_CS
Text GLabel 7100 7700 3    50   Input ~ 0
EPAPIER_RST
Text GLabel 7000 7700 3    50   Input ~ 0
EPAPIER_BUSY
$Comp
L Device:LED D1
U 1 1 62E4DAC2
P 8050 2000
F 0 "D1" H 8043 1745 50  0000 C CNN
F 1 "LED" H 8043 1836 50  0000 C CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8050 2000 50  0001 C CNN
F 3 "~" H 8050 2000 50  0001 C CNN
	1    8050 2000
	-1   0    0    1   
$EndComp
$Comp
L Device:R R3
U 1 1 62E4DAC8
P 7750 2000
F 0 "R3" V 7957 2000 50  0000 C CNN
F 1 "360" V 7866 2000 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7680 2000 50  0001 C CNN
F 3 "~" H 7750 2000 50  0001 C CNN
	1    7750 2000
	0    -1   -1   0   
$EndComp
Text GLabel 8300 2100 3    50   Input ~ 0
GND
Wire Wire Line
	8200 2000 8300 2000
Wire Wire Line
	8300 2000 8300 2100
Text GLabel 7600 2000 0    50   Input ~ 0
HEART_BIT_OUTDOOR
Wire Notes Line
	8850 2800 8850 1500
Text Notes 6750 2800 0    50   ~ 0
HEART BIT OUTDOOR
Text GLabel 7300 5000 1    50   Input ~ 0
HEART_BIT_OUTDOOR
$Comp
L Device:R R2
U 1 1 62E5210E
P 5650 6500
F 0 "R2" V 5443 6500 50  0000 C CNN
F 1 "10k" V 5534 6500 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5580 6500 50  0001 C CNN
F 3 "~" H 5650 6500 50  0001 C CNN
	1    5650 6500
	0    1    1    0   
$EndComp
Wire Wire Line
	6000 6500 5800 6500
Text GLabel 5500 6500 0    50   Input ~ 0
VCC_3V3
$Comp
L Connector:Conn_01x05_Male J9
U 1 1 62E5569C
P 9350 1700
F 0 "J9" V 9412 1944 50  0000 L CNN
F 1 "PROGRAM" V 9503 1944 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 9350 1700 50  0001 C CNN
F 3 "~" H 9350 1700 50  0001 C CNN
	1    9350 1700
	0    1    1    0   
$EndComp
Text GLabel 9250 1900 3    50   Input ~ 0
SWCLK
Text GLabel 9150 1900 3    50   Input ~ 0
SWDIO
Text GLabel 9350 1900 3    50   Input ~ 0
VCC_5V
Text GLabel 9450 1900 3    50   Input ~ 0
GND
Text GLabel 9550 1900 3    50   Input ~ 0
RESET
Wire Notes Line
	10150 2800 10150 1500
Wire Notes Line
	450  2800 10150 2800
Text Notes 8900 2750 0    50   ~ 0
PROGRAM
Text GLabel 7400 5000 1    50   Input ~ 0
SWCLK
Text GLabel 7700 6100 2    50   Input ~ 0
SWDIO
Text GLabel 5800 6500 1    50   Input ~ 0
RESET
$Comp
L Connector:Conn_01x07_Male J7
U 1 1 62DE43B7
P 4700 1700
F 0 "J7" V 4535 1678 50  0000 C CNN
F 1 "SSD1306" V 4626 1678 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 4700 1700 50  0001 C CNN
F 3 "~" H 4700 1700 50  0001 C CNN
	1    4700 1700
	0    1    1    0   
$EndComp
$Comp
L Device:CP C16
U 1 1 62E959ED
P 11300 1000
F 0 "C16" H 11418 1046 50  0000 L CNN
F 1 "100u" H 11418 955 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_5x5.4" H 11338 850 50  0001 C CNN
F 3 "~" H 11300 1000 50  0001 C CNN
	1    11300 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	11050 850  11300 850 
Connection ~ 11050 850 
Wire Wire Line
	11050 1150 11300 1150
Connection ~ 11050 1150
$Comp
L Device:C C6
U 1 1 62E5B03B
P 8350 1050
F 0 "C6" H 8465 1096 50  0000 L CNN
F 1 "100n" H 8465 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 8388 900 50  0001 C CNN
F 3 "~" H 8350 1050 50  0001 C CNN
	1    8350 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 62E5CD5E
P 8750 1050
F 0 "C8" H 8865 1096 50  0000 L CNN
F 1 "100n" H 8865 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 8788 900 50  0001 C CNN
F 3 "~" H 8750 1050 50  0001 C CNN
	1    8750 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C9
U 1 1 62E5DB30
P 8950 1050
F 0 "C9" H 9065 1096 50  0000 L CNN
F 1 "100n" H 9065 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 8988 900 50  0001 C CNN
F 3 "~" H 8950 1050 50  0001 C CNN
	1    8950 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C12
U 1 1 62E5EA0A
P 9200 1050
F 0 "C12" H 9315 1096 50  0000 L CNN
F 1 "100n" H 9315 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 9238 900 50  0001 C CNN
F 3 "~" H 9200 1050 50  0001 C CNN
	1    9200 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C13
U 1 1 62E5F6CB
P 10650 1000
F 0 "C13" H 10765 1046 50  0000 L CNN
F 1 "100n" H 10765 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 10688 850 50  0001 C CNN
F 3 "~" H 10650 1000 50  0001 C CNN
	1    10650 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C15
U 1 1 62E60464
P 10850 1000
F 0 "C15" H 10965 1046 50  0000 L CNN
F 1 "100n" H 10965 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 10888 850 50  0001 C CNN
F 3 "~" H 10850 1000 50  0001 C CNN
	1    10850 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C17
U 1 1 62E61BF6
P 12500 1000
F 0 "C17" H 12615 1046 50  0000 L CNN
F 1 "100n" H 12615 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 12538 850 50  0001 C CNN
F 3 "~" H 12500 1000 50  0001 C CNN
	1    12500 1000
	1    0    0    -1  
$EndComp
Text GLabel 4900 1900 3    50   Input ~ 0
VCC_3V3
Text GLabel 6400 1900 3    50   Input ~ 0
VCC_3V3
$Comp
L Device:C C7
U 1 1 62E5BE49
P 8550 1050
F 0 "C7" H 8665 1096 50  0000 L CNN
F 1 "100n" H 8665 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 8588 900 50  0001 C CNN
F 3 "~" H 8550 1050 50  0001 C CNN
	1    8550 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 1200 9200 1200
Connection ~ 9450 1200
Wire Wire Line
	9200 1200 8950 1200
Connection ~ 9200 1200
Wire Wire Line
	8750 1200 8950 1200
Connection ~ 8950 1200
Wire Wire Line
	8750 1200 8550 1200
Connection ~ 8750 1200
Wire Wire Line
	8550 1200 8350 1200
Connection ~ 8550 1200
Wire Wire Line
	8350 900  8550 900 
Wire Wire Line
	8550 900  8750 900 
Connection ~ 8550 900 
Wire Wire Line
	8750 900  8950 900 
Connection ~ 8750 900 
Wire Wire Line
	8950 900  9200 900 
Connection ~ 8950 900 
$Comp
L ABS25-32.768KHZ-6-T:ABS25-32.768KHZ-6-T Y1
U 1 1 62EAA395
P 11400 2100
F 0 "Y1" H 12100 2365 50  0000 C CNN
F 1 "ABS25-32.768KHZ-6-T" H 12100 2274 50  0000 C CNN
F 2 "Projekt:ABS2532768KHZT" H 12650 2200 50  0001 L CNN
F 3 "https://datasheet.datasheetarchive.com/originals/distributors/Datasheets-DGA13/1170823.pdf" H 12650 2100 50  0001 L CNN
F 4 "32.768 kHz LOW FREQUENCY SMD CRYSTAL" H 12650 2000 50  0001 L CNN "Description"
F 5 "2" H 12650 1900 50  0001 L CNN "Height"
F 6 "ABRACON" H 12650 1800 50  0001 L CNN "Manufacturer_Name"
F 7 "ABS25-32.768KHZ-6-T" H 12650 1700 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "815-ABS25-32.768K6-T" H 12650 1600 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.co.uk/ProductDetail/ABRACON/ABS25-32.768KHZ-6-T?qs=wl%2FhatdkTudHMJeSjMoOzg%3D%3D" H 12650 1500 50  0001 L CNN "Mouser Price/Stock"
F 10 "ABS25-32.768KHZ-6-T" H 12650 1400 50  0001 L CNN "Arrow Part Number"
F 11 "https://www.arrow.com/en/products/abs25-32.768khz-6-t/abracon?region=nac" H 12650 1300 50  0001 L CNN "Arrow Price/Stock"
	1    11400 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C18
U 1 1 62EAB9A6
P 10800 2000
F 0 "C18" H 10915 2046 50  0000 L CNN
F 1 "4,7p" H 10915 1955 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 10838 1850 50  0001 C CNN
F 3 "~" H 10800 2000 50  0001 C CNN
	1    10800 2000
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C19
U 1 1 62EACB01
P 10800 2300
F 0 "C19" H 10915 2346 50  0000 L CNN
F 1 "4,7p" H 10915 2255 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 10838 2150 50  0001 C CNN
F 3 "~" H 10800 2300 50  0001 C CNN
	1    10800 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11400 2100 11150 2100
Wire Wire Line
	11150 2100 11150 2000
Wire Wire Line
	11150 2000 10950 2000
Wire Wire Line
	11400 2200 11150 2200
Wire Wire Line
	11150 2200 11150 2300
Wire Wire Line
	11150 2300 10950 2300
Wire Wire Line
	10650 2300 10650 2000
Text GLabel 10650 2150 0    50   Input ~ 0
GND
Text GLabel 11050 2300 3    50   Input ~ 0
OSC32_IN
Text GLabel 10950 2000 3    50   Input ~ 0
OSC32_OUT
Wire Wire Line
	12800 2100 12800 2200
Text GLabel 6000 6200 0    50   Input ~ 0
OSC32_OUT
Text GLabel 6000 6100 0    50   Input ~ 0
OSC32_IN
$Comp
L Device:C C20
U 1 1 62EFEC08
P 9950 1050
F 0 "C20" H 10065 1096 50  0000 L CNN
F 1 "100n" H 10065 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 9988 900 50  0001 C CNN
F 3 "~" H 9950 1050 50  0001 C CNN
	1    9950 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C21
U 1 1 62F000CE
P 10150 1050
F 0 "C21" H 10265 1096 50  0000 L CNN
F 1 "100n" H 10265 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 10188 900 50  0001 C CNN
F 3 "~" H 10150 1050 50  0001 C CNN
	1    10150 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C22
U 1 1 62F014AA
P 10350 1050
F 0 "C22" H 10465 1096 50  0000 L CNN
F 1 "100n" H 10465 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 10388 900 50  0001 C CNN
F 3 "~" H 10350 1050 50  0001 C CNN
	1    10350 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 900  9950 900 
Connection ~ 9750 900 
Wire Wire Line
	9950 900  10150 900 
Connection ~ 9950 900 
Wire Wire Line
	10150 900  10350 900 
Connection ~ 10150 900 
Wire Wire Line
	9750 1200 9950 1200
Connection ~ 9750 1200
Wire Wire Line
	9950 1200 10150 1200
Connection ~ 9950 1200
Wire Wire Line
	10150 1200 10350 1200
Connection ~ 10150 1200
$Comp
L Device:C C23
U 1 1 62F0F704
P 2850 2550
F 0 "C23" H 2965 2596 50  0000 L CNN
F 1 "100n" H 2965 2505 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 2888 2400 50  0001 C CNN
F 3 "~" H 2850 2550 50  0001 C CNN
	1    2850 2550
	1    0    0    -1  
$EndComp
Text GLabel 2850 2700 0    50   Input ~ 0
ENCODER_SW
Text GLabel 2850 2400 2    50   Input ~ 0
GND
$Comp
L Connector:Conn_01x02_Male J12
U 1 1 6313BA73
P 15150 1850
F 0 "J12" H 15258 2031 50  0000 C CNN
F 1 "Conn_01x02_Male" H 15258 1940 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 15150 1850 50  0001 C CNN
F 3 "~" H 15150 1850 50  0001 C CNN
	1    15150 1850
	1    0    0    -1  
$EndComp
Text GLabel 15350 1950 2    50   Input ~ 0
GND
Text GLabel 15350 1850 2    50   Input ~ 0
VBAT
$Comp
L Device:C C24
U 1 1 63142BD5
P 13750 2200
F 0 "C24" H 13865 2246 50  0000 L CNN
F 1 "100n" H 13865 2155 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 13788 2050 50  0001 C CNN
F 3 "~" H 13750 2200 50  0001 C CNN
	1    13750 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C26
U 1 1 63142BDB
P 14150 2200
F 0 "C26" H 14265 2246 50  0000 L CNN
F 1 "100n" H 14265 2155 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 14188 2050 50  0001 C CNN
F 3 "~" H 14150 2200 50  0001 C CNN
	1    14150 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C27
U 1 1 63142BE1
P 14350 2200
F 0 "C27" H 14465 2246 50  0000 L CNN
F 1 "100n" H 14465 2155 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 14388 2050 50  0001 C CNN
F 3 "~" H 14350 2200 50  0001 C CNN
	1    14350 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C25
U 1 1 63142BE7
P 13950 2200
F 0 "C25" H 14065 2246 50  0000 L CNN
F 1 "100n" H 14065 2155 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 13988 2050 50  0001 C CNN
F 3 "~" H 13950 2200 50  0001 C CNN
	1    13950 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	13750 2050 13950 2050
Wire Wire Line
	13950 2050 14150 2050
Connection ~ 13950 2050
Wire Wire Line
	14150 2050 14350 2050
Connection ~ 14150 2050
Wire Wire Line
	13750 2350 13950 2350
Wire Wire Line
	13950 2350 14150 2350
Connection ~ 13950 2350
Wire Wire Line
	14150 2350 14350 2350
Connection ~ 14150 2350
Text GLabel 14050 2350 3    50   Input ~ 0
GND
Text GLabel 14050 2050 1    50   Input ~ 0
VBAT
Text GLabel 6000 5900 0    50   Input ~ 0
VBAT
$Comp
L Device:R R7
U 1 1 63165B47
P 6700 4100
F 0 "R7" V 6907 4100 50  0000 C CNN
F 1 "10k" V 6816 4100 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 6630 4100 50  0001 C CNN
F 3 "~" H 6700 4100 50  0001 C CNN
	1    6700 4100
	-1   0    0    1   
$EndComp
Text GLabel 6700 3950 0    50   Input ~ 0
GND
Wire Wire Line
	6700 4250 6700 5000
Text GLabel 700  3200 3    50   Input ~ 0
SCREEN_BME_MISO
Text GLabel 800  3200 3    50   Input ~ 0
BME280_CS
Text GLabel 900  3200 3    50   Input ~ 0
SCREEN_BME_MOSI
Text GLabel 1000 3200 3    50   Input ~ 0
SCREEN_BME_SCK
Text GLabel 1600 3200 3    50   Input ~ 0
EPAPIER_SSD1331_DC
Text GLabel 1100 3200 3    50   Input ~ 0
SSD1331_RST
Text GLabel 1200 3200 3    50   Input ~ 0
SSD1331_CS
Text GLabel 1500 3200 3    50   Input ~ 0
EPAPIER_CS
Text GLabel 1300 3200 3    50   Input ~ 0
EPAPIER_RST
Text GLabel 1400 3200 3    50   Input ~ 0
EPAPIER_BUSY
$Comp
L Connector:Conn_01x10_Male J13
U 1 1 63180CA2
P 1200 3000
F 0 "J13" V 1035 2928 50  0000 C CNN
F 1 "Conn_01x10_Male" V 1126 2928 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 1200 3000 50  0001 C CNN
F 3 "~" H 1200 3000 50  0001 C CNN
	1    1200 3000
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x07_Male J14
U 1 1 631A9133
P 2200 3000
F 0 "J14" V 2035 2978 50  0000 C CNN
F 1 "Conn_01x07_Male" V 2126 2978 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x07_P2.54mm_Vertical" H 2200 3000 50  0001 C CNN
F 3 "~" H 2200 3000 50  0001 C CNN
	1    2200 3000
	0    1    1    0   
$EndComp
Text GLabel 2300 3200 3    50   Input ~ 0
MEMORY_MISO
Text GLabel 2400 3200 3    50   Input ~ 0
MEMORY_SCK
Text GLabel 2200 3200 3    50   Input ~ 0
MEMORY_MOSI
Text GLabel 2500 3200 3    50   Input ~ 0
FRAM_HOLD
Text GLabel 2100 3200 3    50   Input ~ 0
FRAM_CS
Text GLabel 2000 3200 3    50   Input ~ 0
FRAM_WP
Text GLabel 1900 3200 3    50   Input ~ 0
FLASH_CS
$Comp
L Connector:Conn_01x01_Male J15
U 1 1 631BD67D
P 3000 3300
F 0 "J15" H 3108 3481 50  0000 C CNN
F 1 "Conn_01x01_Male" H 3108 3390 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 3000 3300 50  0001 C CNN
F 3 "~" H 3000 3300 50  0001 C CNN
	1    3000 3300
	1    0    0    -1  
$EndComp
Text GLabel 3200 3300 2    50   Input ~ 0
GND
$Comp
L Connector:Conn_01x01_Male J16
U 1 1 631C1DBA
P 3000 3550
F 0 "J16" H 3108 3731 50  0000 C CNN
F 1 "Conn_01x01_Male" H 3108 3640 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 3000 3550 50  0001 C CNN
F 3 "~" H 3000 3550 50  0001 C CNN
	1    3000 3550
	1    0    0    -1  
$EndComp
Text GLabel 3200 3550 2    50   Input ~ 0
GND
$Comp
L Connector:Conn_01x01_Male J17
U 1 1 631C3AAF
P 3000 3800
F 0 "J17" H 3108 3981 50  0000 C CNN
F 1 "Conn_01x01_Male" H 3108 3890 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 3000 3800 50  0001 C CNN
F 3 "~" H 3000 3800 50  0001 C CNN
	1    3000 3800
	1    0    0    -1  
$EndComp
Text GLabel 3200 3800 2    50   Input ~ 0
GND
$Comp
L Connector:TestPoint TP1
U 1 1 631C8E48
P 3950 3250
F 0 "TP1" H 4008 3368 50  0000 L CNN
F 1 "TestPoint" H 4008 3277 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_2.0x2.0mm" H 4150 3250 50  0001 C CNN
F 3 "~" H 4150 3250 50  0001 C CNN
	1    3950 3250
	1    0    0    -1  
$EndComp
Text GLabel 3950 3250 3    50   Input ~ 0
GND
Text GLabel 4400 3250 3    50   Input ~ 0
VBAT
Text GLabel 4800 3250 3    50   Input ~ 0
VCC_3V3
Text GLabel 5200 3250 3    50   Input ~ 0
VCC_5V
$Comp
L Connector:TestPoint TP2
U 1 1 631D3C39
P 4400 3250
F 0 "TP2" H 4458 3368 50  0000 L CNN
F 1 "TestPoint" H 4458 3277 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_2.0x2.0mm" H 4600 3250 50  0001 C CNN
F 3 "~" H 4600 3250 50  0001 C CNN
	1    4400 3250
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP3
U 1 1 631D579B
P 4800 3250
F 0 "TP3" H 4858 3368 50  0000 L CNN
F 1 "TestPoint" H 4858 3277 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_2.0x2.0mm" H 5000 3250 50  0001 C CNN
F 3 "~" H 5000 3250 50  0001 C CNN
	1    4800 3250
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP4
U 1 1 631D74AB
P 5200 3250
F 0 "TP4" H 5258 3368 50  0000 L CNN
F 1 "TestPoint" H 5258 3277 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_2.0x2.0mm" H 5400 3250 50  0001 C CNN
F 3 "~" H 5400 3250 50  0001 C CNN
	1    5200 3250
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J18
U 1 1 631E0D7A
P 6000 3200
F 0 "J18" H 6108 3381 50  0000 C CNN
F 1 "Conn_01x01_Male" H 6108 3290 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 6000 3200 50  0001 C CNN
F 3 "~" H 6000 3200 50  0001 C CNN
	1    6000 3200
	1    0    0    -1  
$EndComp
Text GLabel 6200 3200 2    50   Input ~ 0
GND
$Comp
L Connector:Conn_01x01_Male J19
U 1 1 631E2935
P 6050 3500
F 0 "J19" H 6158 3681 50  0000 C CNN
F 1 "Conn_01x01_Male" H 6158 3590 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 6050 3500 50  0001 C CNN
F 3 "~" H 6050 3500 50  0001 C CNN
	1    6050 3500
	1    0    0    -1  
$EndComp
Text GLabel 6250 3500 2    50   Input ~ 0
GND
$Comp
L Connector:Conn_01x01_Male J20
U 1 1 631EA75A
P 3950 3900
F 0 "J20" H 4058 4081 50  0000 C CNN
F 1 "Conn_01x01_Male" H 4058 3990 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 3950 3900 50  0001 C CNN
F 3 "~" H 3950 3900 50  0001 C CNN
	1    3950 3900
	1    0    0    -1  
$EndComp
Text GLabel 4150 3900 2    50   Input ~ 0
DS18B20
$Comp
L Connector:Conn_01x02_Male J21
U 1 1 631EF506
P 4900 3950
F 0 "J21" H 5008 4131 50  0000 C CNN
F 1 "Conn_01x02_Male" H 5008 4040 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4900 3950 50  0001 C CNN
F 3 "~" H 4900 3950 50  0001 C CNN
	1    4900 3950
	1    0    0    -1  
$EndComp
Text GLabel 5100 3950 2    50   Input ~ 0
HC12_RX
Text GLabel 5100 4050 2    50   Input ~ 0
HC12_TX
$Comp
L Connector:Conn_01x02_Male J22
U 1 1 631F711C
P 5600 3800
F 0 "J22" H 5708 3981 50  0000 C CNN
F 1 "Conn_01x02_Male" H 5708 3890 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5600 3800 50  0001 C CNN
F 3 "~" H 5600 3800 50  0001 C CNN
	1    5600 3800
	1    0    0    -1  
$EndComp
Text GLabel 5800 3800 2    50   Input ~ 0
PMS3003_RX
Text GLabel 5800 3900 2    50   Input ~ 0
PMS3003_TX
$EndSCHEMATC
