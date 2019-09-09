# M41T62_HTS221_MS5637_M24M02

![m41t62_hts221 1](https://cloud.githubusercontent.com/assets/6698410/5943657/c43844e8-a6d5-11e4-9e36-93ca1811b901.jpg) ![](https://cloud.githubusercontent.com/assets/6698410/5943689/0289d374-a6d6-11e4-990f-68fd8b07c546.jpg)

Demonstrate M41T62 Real Time Clock operation including define registers, read time and date, set alarms,
configure the interrupts, calibrate the clock with temperature compensation. Also read the pressure and
temperature from the MS5637 altimeter, read the relative humidity from the HTS221 humidity sensor, and read from and write to the M24M02DFC 256 kbyte EEPROM.
Sketch runs on the 3.3 V 8 MHz Pro Mini and the Teensy 3.1.
 
This sketch is specifically for the Teensy 3.1 M41T62 RTC Add-On shield with the M41T62 RTC, an MS5637 pressure/temperature sensor, an HTS221 humidity sensor, and an M24M02DFC 2Mkbit (256 kByte) EEPROM as slave all connected via I2C. 
  
This sketch uses SDA/SCL on pins 17/16, respectively, and it uses the Teensy 3.1-specific Wire library i2c_t3.h.
The MS5637 is a simple but high resolution pressure sensor, which can be used in its high resolution
mode but with power consumption of 20 microAmp, or in a lower resolution mode with power consumption of
only 1 microAmp. The choice will depend on the application.
 
SDA and SCL should have external pull-up resistors (to 3.3V).
4k7 resistors are on the M41T62+HTS221+MS5637+M24512DFM Mini Add-On board for Teensy 3.1.
 
The M41T62+MS5637+HTS221+M24M02DFC board is so low power it can run off of digitalWrite on any GPIO pin. On the Teensy 3.1:
 
 Hardware setup:
 
 M41T62 Mini Add-On ------- Teensy 3.1 
 
 VDD ----------------------  10 digitalWrite(HIGH) 
 
 SDA ----------------------- 17
 
 SCL ----------------------- 16
 
 GND ----------------------  15 digitalWrite(LOW)
 
 IRQ------------------------  9
 
 SW-------------------------  8
 
