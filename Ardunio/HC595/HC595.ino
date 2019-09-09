#include <Wire.h>
#define LM73_ADDR    0x48
  int latchPin = 9;
  int clockPin = 10;
  int dataPin = 8;
  
const byte kEd7sDigitMap[10] = {
  0xC0,0xF9,0xA4,0xB0,0x99,
  0x92,0x82,0xF8,0x80,0x90
};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
   Wire.begin();
   Wire.beginTransmission(LM73_ADDR);
   Wire.write(0x04);                  //Control/Status Register
   Wire.write(0x40);                  //temperature data format 13bits 25C=0x0C80h 150C=4B00h -0.0625C=FFF8h;
   Wire.endTransmission();
   delay(10);

   for(int i=0; i<3; i++)
   {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, kEd7sDigitMap[0]);
    shiftOut(dataPin, clockPin, MSBFIRST, kEd7sDigitMap[0]);
    digitalWrite(latchPin, HIGH);
    delay(350);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0xff);
    shiftOut(dataPin, clockPin, MSBFIRST, 0xff);
    digitalWrite(latchPin, HIGH);
    delay(450);
   }   
}

void loop() {
   unsigned data[2];
   int cTemp;
  Wire.beginTransmission(LM73_ADDR);
  Wire.write(0X00);
  Wire.endTransmission();
  delay(50);
  Wire.requestFrom(LM73_ADDR, 2 );
    if (Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();   
  }
  delay(5);
    int temp  =  ((data[0] * 256) + data[1]) / 8;
     if(temp > 4095)
     {
     temp -= 8192 ;
     }
     
     cTemp = temp * 0.0625;
     
  if (cTemp <= -10)
  {  
    byte data;
    data = 256 - cTemp;
    data = (data/10*16)+(data%10);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, kEd7sDigitMap[data&0x0F]);
    shiftOut(dataPin, clockPin, MSBFIRST, kEd7sDigitMap[data>>4]);
    digitalWrite(latchPin, HIGH); 
        delay(350);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0xff);
    shiftOut(dataPin, clockPin, MSBFIRST, 0xff);
    digitalWrite(latchPin, HIGH); 
  }
  
  if(cTemp <  0 && cTemp > -10)
  {
    byte data;
    data= 256 - cTemp;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, kEd7sDigitMap[data]);
    shiftOut(dataPin, clockPin, MSBFIRST, 0xBF);        // Minus Sign
    digitalWrite(latchPin, HIGH);
  }
  
  if ( cTemp<10 && cTemp>= 0 )
  {
    byte data;
    data = (temp&0x0F) * 0.625;
    if (temp < 0)
    {
    digitalWrite (latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, kEd7sDigitMap[0]);
    shiftOut(dataPin, clockPin, MSBFIRST, 0xBF); // 
    digitalWrite(latchPin, HIGH);    
    }
    else
    {
    digitalWrite (latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, kEd7sDigitMap[data]);
    shiftOut(dataPin, clockPin, MSBFIRST, kEd7sDigitMap[cTemp] & 0x7F); // Decimal point
    digitalWrite(latchPin, HIGH);  
  }
  }
  
 if (cTemp >=10 )
 {
  byte data;
    data = (cTemp/10*16)+(cTemp%10); 
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, kEd7sDigitMap[data&0x0F]);
    shiftOut(dataPin, clockPin, MSBFIRST, kEd7sDigitMap[data>>4]);
    digitalWrite(latchPin, HIGH);
  }  
    delay(500);   
}
