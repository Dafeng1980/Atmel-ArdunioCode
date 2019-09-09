#include<Wire.h>
#define Addr 0x49

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Wire.beginTransmission(Addr);
  Wire.write(0x01);
  Wire.write(0x60);
  Wire.write(0xA0);
  Wire.endTransmission();
  delay(300);
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
  delay(1500);
  // put your main code here, to run repeatedly:

}
