#include <Wire.h>
#include <i2cdetect.h>
int n=0;
void setup()
{
  delay(500);
  Wire.begin();
  Serial.begin(9600);
  delay(500);
  Serial.println("i2cdetect example\n");
  delay(500);
  Serial.print("Scanning address range 0x03-0x77\n\n");
}

void loop() {
  i2cdetect();  // default range from 0x03 to 0x77
  delay(2000);
  n++;
  Serial.print("Scanning Times:");
  Serial.println(n);
  if (n>1000){
    n=0;
  }
  
}
