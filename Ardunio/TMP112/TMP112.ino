
#include<Wire.h>
#define Addr 0x49
const int redOutPin = 5;
const int  greenOutPin = 6;
const int  blueOutPin = 3;

void setup() {
  Wire.begin();
  Serial.begin(9600);       
  Wire.beginTransmission(Addr);
  Wire.write(0x01);
  Wire.write(0x60);
  Wire.write(0xA0);
  Wire.endTransmission();
  delay(300);
}

void loop() {
  unsigned data[2];
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
  if (tempscale< 21)
  { 
  analogWrite(redOutPin, 0);
  analogWrite(greenOutPin, 0);
  analogWrite(blueOutPin, 250);
  }
  else if (tempscale>=21 || tempscale<26)
  {  
  analogWrite(redOutPin, 0);
  analogWrite(greenOutPin, 250);
  analogWrite(blueOutPin, 50);
  }
 else if (tempscale>=26 || tempscale<31)
 {
  analogWrite(redOutPin, 0);
  analogWrite(greenOutPin, 250);
  analogWrite(blueOutPin, 0);
 }
  delay(550);
  }
 
