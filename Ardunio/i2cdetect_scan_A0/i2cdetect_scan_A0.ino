#include <Wire.h>
#include <m24c02read.h>
int n=0;
void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("24C02 EEPROM_READ");
  Serial.print("address range 0x00-0xFF\n\n");
}

void loop() {
  i2cread(0,200);  // default range from 0x03 to 0x77
  delay(3000);
  n++;
  Serial.print("Scanning Times:");
  Serial.println(n);
  if (n>1000){
    n=0;
  }
  
}
