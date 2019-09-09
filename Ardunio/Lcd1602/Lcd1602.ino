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

#define M41T62_FRAC_SEC  0x00
#define M41T62_SECS      0x01
#define M41T62_MINS      0x02
#define M41T62_HRS       0x03
#define M41T62_DAY       0x04
#define M41T62_DATE      0x05
#define M41T62_MNTH      0x06
#define M41T62_YEAR      0x07
#define M41T62_CAL       0x08
#define M41T62_WDOG      0x09
#define M41T62_AL_MNTH   0x0A
#define M41T62_AL_DATE   0x0B
#define M41T62_AL_HR     0x0C
#define M41T62_AL_MIN    0x0D
#define M41T62_AL_SEC    0x0E
#define M41T62_FLAGS     0x0F
#define M41T62_ADDRESS             0x68

#define TMP_ADDR 0x49

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
int backLight = 13; 
void setup() {
   Wire.begin();
   Wire.beginTransmission(TMP_ADDR);
   Wire.write(0x01);
   Wire.write(0x60);
   Wire.write(0xA0);
   Wire.endTransmission();
   delay(100);
   //writeByte(M41T62_ADDRESS,M41T62_SECS,0x80);
   //writeByte(M41T62_ADDRESS,M41T62_MINS,0x32);
   //writeByte(M41T62_ADDRESS,M41T62_HRS,0x20);
   //writeByte(M41T62_ADDRESS,M41T62_DAY,0x07);
  // writeByte(M41T62_ADDRESS,M41T62_DATE,0x16);
   //writeByte(M41T62_ADDRESS,M41T62_MNTH,0x10);
   //writeByte(M41T62_ADDRESS,M41T62_YEAR,0x16);
   //writeByte(M41T62_ADDRESS,M41T62_SECS,0x00);
  
   pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH);
  // set up the LCD's number of columns and rows:
   lcd.begin(16, 2);
  // Print a message to the LCD.
   //lcd.setCursor(1, 0);
  //lcd.print("hello,");
}

void loop() {
  lcd.setCursor(0, 0);
  uint8_t day = (readByte(M41T62_ADDRESS, M41T62_DAY) & 0x07);
    if(day == 1) lcd.print("Mon ");
    if(day == 2) lcd.print("Tue ");   
    if(day == 3) lcd.print("Wed ");
    if(day == 4) lcd.print("Thr ");
    if(day == 5) lcd.print("Fri ");   
    if(day == 6) lcd.print("Sat ");
    if(day == 7) lcd.print("Sun ");
  
     uint8_t month = (10*((readByte(M41T62_ADDRESS, M41T62_MNTH) & 0x10) >> 4) + (readByte(M41T62_ADDRESS, M41T62_MNTH) & 0x0F));
     uint8_t date = (10*((readByte(M41T62_ADDRESS, M41T62_DATE) & 0x30) >> 4) + (readByte(M41T62_ADDRESS, M41T62_DATE) & 0x0F));
     uint8_t year = (10*((readByte(M41T62_ADDRESS, M41T62_YEAR) & 0xF0) >> 4) + (readByte(M41T62_ADDRESS, M41T62_YEAR) & 0x0F));
     uint8_t century = (readByte(M41T62_ADDRESS, M41T62_MNTH) & 0xC0) >> 6;
     lcd.print(month); lcd.print("/"); 
     lcd.print(date); lcd.print("/2"); 
     lcd.print(century); lcd.print(year);
     
     lcd.setCursor(0, 1);
     uint8_t hour = (10*((readByte(M41T62_ADDRESS, M41T62_HRS) & 0x30) >> 4) + (readByte(M41T62_ADDRESS, M41T62_HRS) & 0x0F));
     uint8_t mins = (10*((readByte(M41T62_ADDRESS, M41T62_MINS) & 0x70) >> 4) + (readByte(M41T62_ADDRESS, M41T62_MINS) & 0x0F));
     uint8_t sec = (10*((readByte(M41T62_ADDRESS, M41T62_SECS) & 0x70) >> 4) + (readByte(M41T62_ADDRESS, M41T62_SECS) & 0x0F));
     lcd.print(hour); 
     if(mins < 10) lcd.print(":0"); else lcd.print(":"); lcd.print(mins); 
     if(sec < 10) lcd.print(":0"); else lcd.print(":"); lcd.print(sec);
  delay(300);
  unsigned data[2];
  Wire.beginTransmission(TMP_ADDR);
  Wire.write(0X00);
  Wire.endTransmission();
  delay(300);
  
  Wire.requestFrom(TMP_ADDR, 2 );
    if (Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  int temp  =  ((data[0] * 256) + data[1]) / 16;      // (msb << 8 |lsb) >> 4;
  if (temp >=2048) {
    temp -= 4096;
  }
    float cTemp = temp * 0.0625;
  float fTemp = cTemp * 1.8 + 32;
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(10, 1);  // print the number of seconds since reset:
  // lcd.print(F("Temp: "));
  lcd.print(cTemp, 1);
  lcd.write(0xDF);
  lcd.print("C");
   delay(300);
}

 void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

 uint8_t readByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data;                              // `data` will store the register data   
  Wire.beginTransmission(address);         // Initialize the Tx buffer
  Wire.write(subAddress);                  // Put slave register address in Tx buffer
  Wire.endTransmission();        // Send the Tx buffer, but send a restart to keep connection alive
//  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
//  Wire.requestFrom(address, 1);  // Read one byte from slave register address 
  Wire.requestFrom(address, (size_t) 1);   // Read one byte from slave register address 
  data = Wire.read();                      // Fill Rx buffer with result
  return data;                             // Return data read from slave register
}


