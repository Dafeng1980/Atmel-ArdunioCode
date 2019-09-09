/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <Wire.h>

#define LM73_ADDR    0x48
#define TMP112_ADDR  0x49

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(26, 27, 28, 29, 30, 31);

int backLight = 22; 
void setup() {
   Wire.begin();
   Wire.beginTransmission(LM73_ADDR);
   Wire.write(0x01);
   Wire.write(0x60);
   Wire.endTransmission();
   delay(10);
   Wire.beginTransmission(LM73_ADDR);
   Wire.write(0x04);
   Wire.write(0x40);
   Wire.endTransmission();
   delay(10);
   Wire.beginTransmission(TMP112_ADDR);
   Wire.write(0x01);
   Wire.write(0x60);
   Wire.write(0xA0);
   Wire.endTransmission();
  delay(20);  
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH);
  // set up the LCD's number of columns and rows:
   lcd.begin(16, 2);
}
void loop() {
  unsigned data[4];
  Wire.beginTransmission(LM73_ADDR);
  Wire.write(0X00);
  Wire.endTransmission();
  delay(50);
  Wire.requestFrom(LM73_ADDR, 2 );
    if (Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  int temp  =  ((data[0] * 256) + data[1]) / 8;      // (msb << 8 |lsb) >> 3;
  if (temp >= 4096) {
    temp -= 8192;
  }
    float cTemp = temp * 0.0625;
    
 // float fTemp = cTemp * 1.8 + 32;
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);  // print the number of seconds since reset:
  lcd.print(F("Temp Ext: "));
  lcd.print(cTemp, 1);
  lcd.write(0xDF);
  lcd.print("C");
  Wire.beginTransmission(TMP112_ADDR);
  Wire.write(0X00);
  Wire.endTransmission();
  delay(50);
  Wire.requestFrom(TMP112_ADDR, 2 );
    if (Wire.available() == 2) {
    data[2] = Wire.read();
    data[3] = Wire.read();
  }
  int temp1  =  ((data[2] * 256) + data[3]) / 16;      // (msb << 8 |lsb) >> 4;
  if (temp1 >= 2048) {
    temp1 -= 4096;
  }
    float fTemp = temp1 * 0.0625;
  lcd.setCursor(0, 1);
  lcd.print(F("Internal: "));
  lcd.print(fTemp, 1);
  lcd.write(0xDF);
  lcd.print("C"); 
   delay(300);
}



