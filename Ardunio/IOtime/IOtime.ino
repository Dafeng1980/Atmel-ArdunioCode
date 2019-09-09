// #include <Wire.h>
// I2C address of the MAX1139 I2C address is 0x35
//#define MAX1139_ADDR 0x35
#define Pi 3.14


int i = 0;
//float ray;
int ray;
void setup()
  {   
    pinMode (26, OUTPUT);
    //pinMode (0, OUTPUT);
    //pinMode (1, OUTPUT);
    Serial.begin(38400);
    //Wire.begin(); 
   // Wire.beginTransmission(MAX1139_ADDR);  
   // Wire.write(0xF0);
   // Wire.endTransmission();  
    delay(30);   
  }
void loop()
  { 
    
    //float r = 17.72;
    i++;
    if(i==0xFFFF)
    i=0;
  digitalWrite(26, HIGH);
  //ray = i;
 // ray = ray * Pi;
 // Serial.println(ray, 4);
  digitalWrite(26, LOW);
  Serial.println(ray);
  //delayMicroseconds(20);
   }


