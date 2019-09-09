#include<Wire.h>
#define Addr 0x48

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Wire.beginTransmission(Addr);
  Wire.write(0x01);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(100);
  // put your setup code here, to run once:

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
  //int tempscale = data[0];
  int temp = ((data[0] << 8) | data[1]) >> 7;
  if (temp >= 0x100) 
  {
    temp -= 0x1FF + 1;
  }
  float cTemp = temp * 0.5;
  Serial.print("temperature: ");
  Serial.print(cTemp, 1);
  Serial.println(" C");
  delay(2000);
  // put your main code here, to run repeatedly:

}
