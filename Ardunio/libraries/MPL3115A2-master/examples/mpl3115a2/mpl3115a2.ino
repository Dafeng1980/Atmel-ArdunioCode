#include <MPL3115A2.h>
#include <Wire.h>

void setup(){
  Serial.begin(9600);
  MPL3115A2.begin();
}
//LOOP
void loop(){
  Serial.print("Pressure is: ");
  MPL3115A2.getPressure(); //Expressed in Pa
  Serial.print(MPL3115A2._p_msb, HEX);
  Serial.print(' ');
  Serial.print(MPL3115A2._p_csb, HEX);
  Serial.print(' ');
  Serial.println(MPL3115A2._p_lsb, HEX);
  //Serial.print("Altimetry is: ");
  //MPL3115A2.getAltimetry(); //Expressed in m
  Serial.print("Temperature is: ");
  MPL3115A2.getTemp();  //Expressed in C
  Serial.print(MPL3115A2._t_msb, HEX);
  Serial.print(' ');
  Serial.println(MPL3115A2._t_lsb, HEX);
  delay(2000);
}
