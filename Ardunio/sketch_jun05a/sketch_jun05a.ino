//#include <i2c_t3.h>


#include "Wire.h"

#define TMP102_I2C_ADDRESS 73 /* This is the I2C address for our chip.
This value is correct if you tie the ADD0 pin to ground. See the datasheet for some other values. */

void setup() {
  Wire.begin(); // start the I2C library
  Serial.begin(38400); //Start serial communication at 115200 baud
}
void getTemp102(){
  byte firstbyte, secondbyte; //these are the bytes we read from the TMP102 temperature registers
  int val; /* an int is capable of storing two bytes, this is where we "chuck" the two bytes together. */ 
 
 // float convertedtemp; /* We then need to multiply our two bytes by a scaling factor, mentioned in the datasheet. */ 
  // float correctedtemp; 
  // The sensor overreads (?) 
  /* Reset the register pointer (by default it is ready to read temperatures)
You can alter it to a writeable register and alter some of the configuration - 
the sensor is capable of alerting you if the temperature is above or below a specified threshold. */
  Wire.beginTransmission(TMP102_I2C_ADDRESS); //Say hi to the sensor. 
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(TMP102_I2C_ADDRESS, 2);
  firstbyte      = (Wire.read()); 
/*read the TMP102 datasheet - here we read one byte from
 each of the temperature registers on the TMP102*/
  secondbyte     = (Wire.read()); 
/*The first byte contains the most significant bits, and 
 the second the less significant */
    val = ((firstbyte) << 4);  
 /* MSB */
    val |= (secondbyte >> 4);    
/* LSB is ORed into the second 4 bits of our byte.
Bitwise maths is a bit funky, but there's a good tutorial on the playground*/
    // convertedtemp = val*0.0625;
    // correctedtemp = convertedtemp - 5; 
    /* See the above note on overreading */


  Serial.print("firstbyte is ");
  Serial.print("\t");
  Serial.println(firstbyte, BIN);
  Serial.print("secondbyte is ");
  Serial.print("\t");
  Serial.println(secondbyte, BIN);
  Serial.print("Concatenated byte is ");
  Serial.print("\t");
  Serial.println(val, BIN);
  Serial.print("Temp is ");
  Serial.print("\t");
  Serial.println(val*0.0625);
  // Serial.print("Corrected temp is ");
  // Serial.print("\t");
  // Serial.println(correctedtemp);
  Serial.println();
}

void loop() {
  getTemp102();
  delay(2000); //wait 5 seconds before printing our next set of readings. 
}
