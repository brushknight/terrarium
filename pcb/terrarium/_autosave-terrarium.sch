EESchema Schematic File Version 4
EELAYER 30 0
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
L Connector_Generic_MountingPin:Conn_01x19_MountingPin esp32-left1
U 1 1 6186D148
P 3200 2900
F 0 "esp32-left1" H 3288 2864 50  0000 L CNN
F 1 "Conn_01x19_MountingPin" H 3288 2773 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x19_P2.54mm_Vertical" H 3200 2900 50  0001 C CNN
F 3 "~" H 3200 2900 50  0001 C CNN
	1    3200 2900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic_MountingPin:Conn_01x19_MountingPin esp32-right1
U 1 1 6186E59A
P 4700 2900
F 0 "esp32-right1" H 4788 2864 50  0000 L CNN
F 1 "Conn_01x19_MountingPin" H 4788 2773 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x19_P2.54mm_Vertical" H 4700 2900 50  0001 C CNN
F 3 "~" H 4700 2900 50  0001 C CNN
	1    4700 2900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic_MountingPin:Conn_01x03_MountingPin relay1
U 1 1 61870C0F
P 5700 1100
F 0 "relay1" H 5788 1064 50  0000 L CNN
F 1 "1" H 5788 973 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 5700 1100 50  0001 C CNN
F 3 "~" H 5700 1100 50  0001 C CNN
	1    5700 1100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic_MountingPin:Conn_01x03_MountingPin temp4
U 1 1 618718D1
P 4700 1100
F 0 "temp4" H 4788 1064 50  0000 L CNN
F 1 "4" H 4788 973 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4700 1100 50  0001 C CNN
F 3 "~" H 4700 1100 50  0001 C CNN
	1    4700 1100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic_MountingPin:Conn_01x03_MountingPin temp3
U 1 1 61871FA0
P 4050 1100
F 0 "temp3" H 4138 1064 50  0000 L CNN
F 1 "3" H 4138 973 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4050 1100 50  0001 C CNN
F 3 "~" H 4050 1100 50  0001 C CNN
	1    4050 1100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic_MountingPin:Conn_01x03_MountingPin temp2
U 1 1 618725D3
P 3450 1100
F 0 "temp2" H 3538 1064 50  0000 L CNN
F 1 "2" H 3538 973 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3450 1100 50  0001 C CNN
F 3 "~" H 3450 1100 50  0001 C CNN
	1    3450 1100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic_MountingPin:Conn_01x03_MountingPin temp1
U 1 1 61872C21
P 2850 1100
F 0 "temp1" H 2938 1064 50  0000 L CNN
F 1 "1" H 2938 973 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 2850 1100 50  0001 C CNN
F 3 "~" H 2850 1100 50  0001 C CNN
	1    2850 1100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 i2c1
U 1 1 618737F4
P 1550 1200
F 0 "i2c1" H 1630 1192 50  0000 L CNN
F 1 "RTC" H 1630 1101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1550 1200 50  0001 C CNN
F 3 "~" H 1550 1200 50  0001 C CNN
	1    1550 1200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 i2c2
U 1 1 618742D4
P 1550 1950
F 0 "i2c2" H 1630 1942 50  0000 L CNN
F 1 "LCD" H 1630 1851 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1550 1950 50  0001 C CNN
F 3 "~" H 1550 1950 50  0001 C CNN
	1    1550 1950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic_MountingPin:Conn_01x03_MountingPin relay2
U 1 1 6187A97E
P 6700 1100
F 0 "relay2" H 6788 1064 50  0000 L CNN
F 1 "2" H 6788 973 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6700 1100 50  0001 C CNN
F 3 "~" H 6700 1100 50  0001 C CNN
	1    6700 1100
	1    0    0    -1  
$EndComp
Text GLabel 6500 1000 0    50   Input ~ 0
GND
Text GLabel 5500 1000 0    50   Input ~ 0
GND
Text GLabel 4500 1000 0    50   Input ~ 0
GND
Text GLabel 3850 1000 0    50   Input ~ 0
GND
Text GLabel 3250 1000 0    50   Input ~ 0
GND
Text GLabel 2650 1000 0    50   Input ~ 0
GND
Text GLabel 1350 1100 0    50   Input ~ 0
GND
Text GLabel 1350 1850 0    50   Input ~ 0
GND
Text GLabel 3000 3800 0    50   Input ~ 0
GND
Text GLabel 4500 2000 0    50   Input ~ 0
VCC
Text GLabel 4500 3800 0    50   Input ~ 0
3v3
Text GLabel 5500 1100 0    50   Input ~ 0
VCC
Text GLabel 6500 1100 0    50   Input ~ 0
VCC
Text GLabel 4500 1200 0    50   Input ~ 0
VCC
Text GLabel 3850 1200 0    50   Input ~ 0
VCC
Text GLabel 3250 1200 0    50   Input ~ 0
VCC
Text GLabel 2650 1200 0    50   Input ~ 0
VCC
Text GLabel 1350 1950 0    50   Input ~ 0
VCC
Text GLabel 1350 1200 0    50   Input ~ 0
3v3
Text GLabel 1350 1300 0    50   Input ~ 0
SDA
Text GLabel 1350 1400 0    50   Input ~ 0
SCL
Text GLabel 1350 2050 0    50   Input ~ 0
SDA
Text GLabel 1350 2150 0    50   Input ~ 0
SCL
Text GLabel 3000 3600 0    50   Input ~ 0
SCL
Text GLabel 3000 3300 0    50   Input ~ 0
SDA
Text GLabel 3000 3200 0    50   Input ~ 0
GND
Text GLabel 3000 3100 0    50   Input ~ 0
G19
Text GLabel 3000 3000 0    50   Input ~ 0
G18
Text GLabel 3000 2800 0    50   Input ~ 0
G17
Text GLabel 3000 2700 0    50   Input ~ 0
G16
Text GLabel 3000 2600 0    50   Input ~ 0
G4
Text GLabel 3000 2900 0    50   Input ~ 0
G5
Text GLabel 2650 1100 0    50   Input ~ 0
G16
Text GLabel 3250 1100 0    50   Input ~ 0
G17
Text GLabel 3850 1100 0    50   Input ~ 0
G18
Text GLabel 4500 1100 0    50   Input ~ 0
G19
Text GLabel 5500 1200 0    50   Input ~ 0
G4
Text GLabel 6500 1200 0    50   Input ~ 0
G5
NoConn ~ 3000 2000
NoConn ~ 3000 2100
NoConn ~ 3000 2200
NoConn ~ 3000 2300
NoConn ~ 3000 2400
NoConn ~ 3000 2500
NoConn ~ 4500 2100
NoConn ~ 4500 2200
NoConn ~ 4500 2300
NoConn ~ 4500 2400
NoConn ~ 4500 2500
NoConn ~ 4500 2600
NoConn ~ 4500 2700
NoConn ~ 4500 2800
NoConn ~ 4500 2900
NoConn ~ 4500 3000
NoConn ~ 4500 3100
NoConn ~ 4500 3200
NoConn ~ 4500 3300
NoConn ~ 4500 3400
NoConn ~ 4500 3500
NoConn ~ 4500 3600
NoConn ~ 4500 3700
NoConn ~ 3000 3700
NoConn ~ 3000 3500
NoConn ~ 3000 3400
NoConn ~ 6700 1400
NoConn ~ 5700 1400
NoConn ~ 2850 1400
NoConn ~ 3450 1400
NoConn ~ 4050 1400
NoConn ~ 4700 1400
NoConn ~ 3200 4000
NoConn ~ 4700 4000
$Comp
L Connector_Generic:Conn_01x03 ExternalPower1
U 1 1 618715FE
P 1550 2750
F 0 "ExternalPower1" H 1630 2792 50  0000 L CNN
F 1 "Conn_01x03" H 1630 2701 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 1550 2750 50  0001 C CNN
F 3 "~" H 1550 2750 50  0001 C CNN
	1    1550 2750
	1    0    0    -1  
$EndComp
Text GLabel 1350 2650 0    50   Input ~ 0
GND
Text GLabel 1350 2850 0    50   Input ~ 0
VCC
Text GLabel 1350 2750 0    50   Input ~ 0
3v3
$EndSCHEMATC
