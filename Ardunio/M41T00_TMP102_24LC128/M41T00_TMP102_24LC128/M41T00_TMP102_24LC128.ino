/*
 Name:		M41T00_TMP102_24LC128.ino
 Created:	12/16/2017 10:52:16 PM
 Author:	Dafeng
*/
#include <Wire.h>
#include <i2c_rtc_m41t00s.h>
#include <ShiftDisplay.h>

#define TMP112_ADDR  0x49

i2c_rtc_m41t00s rtc;
ShiftDisplay display(COMMON_ANODE, 3);

char dateString[20];
char timeString[20];

// the setup function runs once when you press reset or power the board
void setup() {

	// initialize LCD with number of columns and rows: 
	rtc.begin();
	Wire.beginTransmission(TMP112_ADDR);
	Wire.write(0x01);
	Wire.write(0x60);
	Wire.write(0xA0);
	Wire.endTransmission();
	delay(50);
}

// the loop function runs over and over again until power down or reset
void loop() {

	uint8_t u8Status = rtc.get();
	uint8_t data[2];

	if (!u8Status)
	{
		display.show(rtc.time.sec, 1000);
		sprintf(dateString, " %4u-%02u-%02u ", 2000 + rtc.time.year, rtc.time.month, rtc.time.day);
		sprintf(timeString, "%02u %02u %02u", rtc.time.hour, rtc.time.min, rtc.time.sec);
		// display.show(timeString, 1000);
	}


	Wire.beginTransmission(TMP112_ADDR);
	Wire.write(0X00);
	Wire.endTransmission();
	delay(50);
	Wire.requestFrom(TMP112_ADDR, 2);
	if (Wire.available() == 2) {
		data[0] = Wire.read();
		data[1] = Wire.read();
	}
	int temp1 = ((data[0] * 256) + data[1]) / 16;      // (msb << 8 |lsb) >> 4;
	if (temp1 >= 2048) {
		temp1 -= 4096;
	}
	float fTemp = temp1 * 0.0625;
	//display.show(fTemp, 1000);

	delay(250);
}


