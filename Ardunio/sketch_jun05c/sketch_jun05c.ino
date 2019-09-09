 #include <Wire.h>
// I2C address of the MAX1139 I2C address is 0x35
//#define MAX1139_ADDR 0x35
void setup()
  {   
    pinMode (13, OUTPUT);
    pinMode (0, OUTPUT);
    pinMode (1, OUTPUT);
    //Wire.begin(); 
   // Wire.beginTransmission(MAX1139_ADDR);  
   // Wire.write(0xF0);
   // Wire.endTransmission();  
    delay(1200);   
  }
void loop()
  { 
  digitalWrite(13, HIGH);
  digitalWrite(0, LOW);
  delay(2500);
  digitalWrite(0, HIGH);
  digitalWrite(13,LOW);
  delay(2500);
   }


