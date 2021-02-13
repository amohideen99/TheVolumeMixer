EESchema Schematic File Version 4
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
L MCU_Microchip_SAMD:ATSAMD21E18A-A U?
U 1 1 6027A44B
P 6000 2950
F 0 "U?" H 6629 2996 50  0000 L CNN
F 1 "ATSAMD21E18A-A" H 6629 2905 50  0000 L CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 6900 1100 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/SAM_D21_DA1_Family_Data%20Sheet_DS40001882E.pdf" H 6000 2950 50  0001 C CNN
	1    6000 2950
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AP2127K-3.3 U?
U 1 1 6027AB50
P 7200 2150
F 0 "U?" H 7200 2492 50  0000 C CNN
F 1 "AP2127K-3.3" H 7200 2401 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 7200 2475 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/AP2127.pdf" H 7200 2250 50  0001 C CNN
	1    7200 2150
	1    0    0    -1  
$EndComp
$Comp
L Driver_LED:TLC5940PWP U?
U 1 1 6027AE1C
P 4950 5700
F 0 "U?" H 4950 6878 50  0000 C CNN
F 1 "TLC5940PWP" H 4950 6787 50  0000 C CNN
F 2 "Package_SO:HTSSOP-28-1EP_4.4x9.7mm_P0.65mm_EP3.4x9.5mm_Mask2.4x6.17mm_ThermalVias" H 4975 4725 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/tlc5940.pdf" H 4550 6400 50  0001 C CNN
	1    4950 5700
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW?
U 1 1 6027AEA9
P 8300 5350
F 0 "SW?" H 8300 5717 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 8300 5626 50  0000 C CNN
F 2 "" H 8150 5510 50  0001 C CNN
F 3 "~" H 8300 5610 50  0001 C CNN
	1    8300 5350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
