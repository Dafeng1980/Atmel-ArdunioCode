
#include <Wire.h>
#include <Arduino.h>
#include "i2cdetect.h"

// _printf base code: http://playground.arduino.cc/Main/Printf
#include <stdarg.h>
#define PRINTF_BUF 256 // define the tmp buffer size (change if desired)
void _printf(const char *format, ...)
{
  char buf[PRINTF_BUF];
  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, sizeof(buf), format, ap);
  for(char *p = &buf[0]; *p; p++) // emulate cooked mode for newlines
  {
    if(*p == '\n')
      Serial.write('\r');
    Serial.write(*p);
  }
  va_end(ap);
}

void i2cdetect(uint8_t first, uint8_t last) {
  uint8_t i, j, data; // error;

  // header
  Serial.print("   ");
  for (i = 0; i < 16; i++) {
    printf("%3x", i);
  }
  Serial.println("");

  // for (j = 0; j < 8; j++) {
   //  _printf("%02d:", j*10);
   // for (i = 0; i < 16; i++) {
     // data = i + j*16;
    //  if (data >= first && data <= last) {
       Wire.beginTransmission(0x50);
		Wire.write(first);
       // error = Wire.endTransmission();
       // if (error) {
		   Wire.endTransmission(false);
		   Wire.requestFrom(0x50, last);
		     for (j = 0; j < 16; j++) {
			     _printf("%02x:", j*10);
		       for (i = 0; i < 16; i++) {
			       //data = i + j*16;
		   
         // Serial.print(" --");
       // } else {
		   data=Wire.read();
          _printf(" %02x", data);
      //  }
      } 
	  //else {
        //Serial.print("   ");
      //}
   // }
    Serial.println("");
  }
  
}

void i2cdetect() {
  i2cdetect(0x00, 0xff);
}

