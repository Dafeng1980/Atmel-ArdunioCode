#include "Wire.h"
#define M24AA512_I2C_ADDR  0x50
void setup() {
  
  Wire.begin();
  Serial.begin(38400);
  delay(200);
 // M24AA512wirteByte(M24AA512_I2C_ADDR, 0x00, 0x01, 0xAA);
  M24AA512wirteByte(M24AA512_I2C_ADDR, 0x00, 0x02, 0x3F);
  delay(300);
  
  //Serial.println( d = M24AA512readByte(M24AA512_I2C_ADDR,0x00,0x01);
  // put your setup code here, to run once:
}

void loop() {
  uint8_t d;
  //d = M24AA512readByte(M24AA512_I2C_ADDR,0x00,0x01);
  //Serial.println( d, HEX);
  delay(500);
  d = M24AA512readByte(M24AA512_I2C_ADDR,0x00,0x02);
   Serial.println( d, HEX);
  delay(2000);
  // put your main code here, to run repeatedly:

}
void M24AA512wirteByte (uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint8_t data)
{
  Wire.beginTransmission(device_address);
  Wire.write(data_address1);
  Wire.write(data_address2);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t M24AA512readByte(uint8_t device_address, uint8_t data_address1, uint8_t data_address2) {
  uint8_t data;
  Wire.beginTransmission(device_address);
  Wire.write(data_address1);
  Wire.write(data_address2);
  //Wire.endTransmission(false); // NO Stop, restar.
  Wire.endTransmission();//Stop & Star
  Wire.requestFrom(device_address,(size_t) 1);
  data = Wire.read();
  return data; 
}

