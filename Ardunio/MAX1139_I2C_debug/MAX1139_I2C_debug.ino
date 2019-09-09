#include<Wire.h>
#define MAX1139_ADDR 0x35
void setup() {
    Serial.begin(38400);
     Wire.begin();
    delay(1000);
  // put your setup code here, to run once:
 I2Cscan();
     delay(1500);
     Serial.println("Finish!"); 
}

void loop() {

}
void I2Cscan() 
{
    // scan for i2c devices
  byte error, address, flag;
  int nDevices;

  Serial.println("Scanning...");
   flag=0;
  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
if (address == 0x35) {
   delay(100);
   Wire.beginTransmission(MAX1139_ADDR);  
   Wire.write(0xF0);
   Wire.endTransmission(); 
  // Serial.println("Configuration SetupByte@0xF0 in MAX1139 PIN 13 OUTPUT");
    flag=1;
}
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
      if (flag==0)
    Serial.println("Cannot found device MAX1139 at address 0x35");
    else
    Serial.println("Configuration SetupByte@0xF0 in MAX1139 PIN 13 OUTPUT");
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

    
}
