// Date and time functions using a M41T62 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "M41T62.h"
#define Addr 0x49
RTC_M41T62 rtc;

void setup () {
  Serial.begin(38400);
  
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();
  Wire.beginTransmission(Addr);
  Wire.write(0x01);
  Wire.write(0x60);
  Wire.write(0xA0);
  Wire.endTransmission();

  // following line sets the RTC to the date & time this sketch was compiled
 // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void loop () {
     unsigned data[2];
    DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
  Wire.beginTransmission(Addr);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(Addr,2);
  if (Wire.available() == 2)
  {
    data[0]=Wire.read();
    data[1]=Wire.read();
  }
  int tempscale = data[0];
  int temp = ((data[0] << 8) | data[1]) >> 4;
  if (temp >= 0x800) 
  {
    temp -= 0xFFF + 1;
  }
  float cTemp = temp * 0.0625;
  Serial.print("temperature: ");
  Serial.print(cTemp, 1);
  Serial.println(" C");
     /*
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    // calculate a date which is 7 days and 30 seconds into the future
   DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    Serial.println();
*/
delay(3000);
}
