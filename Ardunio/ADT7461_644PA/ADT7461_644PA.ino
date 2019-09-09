#include<Wire.h>
#define Addr 0x4C

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Wire.beginTransmission(Addr);
  Wire.write(0x09);
  Wire.write(0x00);
 // Wire.write(0xA0);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(Addr);
  Wire.write(0x0A);
  Wire.write(0x05);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(Addr);
  Wire.write(0x0D);
  Wire.write(0x1E);
  Wire.endTransmission();
    
  delay(100);
  // put your setup code here, to run once:

}

void loop() {
    uint8_t data[4];
  Wire.beginTransmission(Addr);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(Addr,1);
  //if (Wire.available() == 2)
  //{
    data[0]=Wire.read();
  Wire.beginTransmission(Addr);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(Addr,1);
  data[1]=Wire.read();
  
  Wire.beginTransmission(Addr);
  Wire.write(0x10);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(Addr,1);
  data[2]=Wire.read();

  Wire.beginTransmission(Addr);
  Wire.write(0x02);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(Addr,1);
  data[3]=Wire.read();

  // if ((data[1]- 64) <= 0)
  //int eTemp = data[1] - 64;
 // float tempd = (data[2] >> 6) * 0.25;
 // float eeTemp = eTemp + tempd;
  
  int extemp=((data[1] << 8) | data[2]) >> 6;
  float eTemp = extemp * 0.25;
   // data[1]=Wire.read();
 // }
 // int tempscale = data[0];
  // int temp = ((data[0] << 8) | data[1]) >> 4;
 // if (temp >= 0x800) 
 // {
 //   temp -= 0xFFF + 1;
//  }
 // float cTemp = temp * 0.0625;
 
 int temp = data[0];
 int limt = data[3];
  Serial.print("temperature: ");
  Serial.print(temp);
  //Serial.print(cTemp, 1);
  Serial.println(" C");
    Serial.print("external temperature: ");
  Serial.print(eTemp, 2);
  Serial.println(" C");
  Serial.print("Dot number: ");
   Serial.println(data[1], HEX);
   Serial.print(" Status:");
   Serial.println(limt, HEX);
  
  //Serial.print(cTemp, 1);
  
  delay(1500);
  // put your main code here, to run repeatedly:

}
