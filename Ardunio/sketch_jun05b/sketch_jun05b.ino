#include <Wire.h>
// #define TMP_ADDR 73
void setup()
  {
    // I2C address of the TMP112
   #define TMP_ADDR 0x49              
    // Join I2c Bus as master
    Wire.begin(); 
    // Start serial communication
    Serial.begin(38400);
    // Begin transmission
    Wire.beginTransmission(TMP_ADDR);  
    // Select ENABLE register
    Wire.write(0x01);
    // Select normal operation
    Wire.write(0x60A0);
    // End transmission and release I2C bus
    Wire.endTransmission();  
  }
void loop()
  {
    // Begin transmission
    Wire.beginTransmission(TMP_ADDR);  
    // Select Data Registers
    Wire.write(0X00);
    // Wire.endTransmission();
    delay(500);                        
    // Request 2 bytes , Msb first  
    Wire.requestFrom(TMP_ADDR, 2 ); 
   // Wire.endTransmission();
    // Read the two bytes
    while(Wire.available()) {                    
    int msb = Wire.read();
    int lsb = Wire.read();
    Wire.endTransmission();
    // Data conversion in raw values
    int rawtmp  =  msb << 8 |lsb;
    int value   =  rawtmp >> 4;
    double ans  =  value * 0.0625;
    // Print output       
    Serial.print("celsius value : ");    
    Serial.println(ans);
   }
   delay(2000);
  }

