#include<Wire.h>
#define U510_ADDR 0x33
#define U500_ADDR 0x2D
byte data;
byte data1;
byte data2;
byte data3;
byte data4;
void setup() {
    Serial.begin(38400);
     Wire.begin();
    delay(100);
    //I2Cscan();
     delay(10);
  Wire.beginTransmission(U510_ADDR);
  Wire.write(0x98);
  Wire.endTransmission(false);
  Wire.requestFrom(U510_ADDR, 1);
  data=Wire.read();
  Wire.endTransmission();
  Serial.print("U510,STATUS_BYTE! = ");
  Serial.print(data,HEX);
  Serial.println(' ');
  delay(10);
  Wire.beginTransmission(U500_ADDR);
  Wire.write(0x9A);
  Wire.endTransmission(false);
  Wire.requestFrom(U500_ADDR, 4);
  data1=Wire.read();
  data2=Wire.read();
  data3=Wire.read();
  data4=Wire.read();
  
  Serial.print("U500,STATUS_BYTE! = "); 
  Serial.print(data1,HEX);
  Serial.print(' ');
  Serial.print(data2,HEX);
  Serial.print(' ');
  Serial.print(data3,HEX);
  Serial.print(' ');
  Serial.print(data4,HEX);
  Serial.println(' ');
  Serial.println("Finish!"); 
}

void loop() {

}
void I2Cscan() 
{
  byte error, address, flag;
  int nDevices;

  Serial.println("Scanning...");
   flag=0;
  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
    
}
