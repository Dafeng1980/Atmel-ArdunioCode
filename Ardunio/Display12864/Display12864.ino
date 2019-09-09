

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


/*
U8glib Example Overview:
Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.

*/

// Please UNCOMMENT one of the contructor lines below

U8G2_SH1106_128X64_VCOMH0_F_HW_I2C u8g2(U8G2_R0);

void setup(void) 
  {
  Serial.begin(9600);
  Serial.println("You Have been connected to computer");
	u8g2.begin();
	u8g2.enableUTF8Print();		// enable UTF8 support for the Arduino print() function
}

void loop(void) {
	int pri = 123;
	u8g2.setFont(u8g2_font_ncenB08_tr);  // use chinese2 for all the glyphs of "�������"
	u8g2.setFontDirection(0);
	u8g2.clearBuffer();
	u8g2.setCursor(0, 10);
	u8g2.print("Hello World!");
	u8g2.print(pri);
	u8g2.setCursor(0, 25);
	u8g2.print("I AM Dafeng1");		// Chinese "Hello World" 
	u8g2.setCursor(0, 40);
     u8g2.print("I AM Happy");
	 u8g2.setCursor(0, 55);
	 u8g2.print("In Today");

	 u8g2.sendBuffer();
	delay(1000);
}
