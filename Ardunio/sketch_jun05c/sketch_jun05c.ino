#include <Wire.h>

// I2C address of the TMP112 I2C address is 0x49(73)
#define TMP_ADDR 0x49
void setup()
{
  // Join I2c Bus as master
  Wire.begin();
  // Start serial communication,set baud rate =9600
  Serial.begin(9600);
  // Begin transmission
  Wire.beginTransmission(TMP_ADDR);
  // Select ENABLE register
  Wire.write(0x01);
  // Select normal operation
  Wire.write(0x60);
  Wire.write(0xA0);
  // End transmission and release I2C bus
  Wire.endTransmission();
}
void loop()
{
  unsigned data[2];
  // Begin transmission
  Wire.beginTransmission(TMP_ADDR);
  // Select Data Registers
  Wire.write(0X00);
  //Stop I2C Transmission
  Wire.endTransmission();
  delay(300);

  // Request 2 bytes of data,
  Wire.requestFrom(TMP_ADDR, 2 );
  // Read the two bytes  temp msb, temp lsb
  if (Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  //  Conver the data to 12-bit
  int temp  =  ((data[0] * 256) + data[1]) / 16;      // (msb << 8 |lsb) >> 4;
  if (temp > 2048) {
    temp -= 4096;
  }
  float cTemp = temp * 0.0625;
  float fTemp = cTemp * 1.8 + 32;

  // Print data to serial monitor
  Serial.print("Celsius value : ");
  Serial.print(cTemp);
  Serial.println(" C");
  Serial.print("Farhenheit value : ");
  Serial.print(fTemp);
  Serial.println(" F");
  delay(1500);
}



