/*
 Name:		M41T00_TMP102_24LC128.ino
 Created:	12/16/2017 10:52:16 PM
 Author:	Dafeng
*/
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <i2c_rtc_m41t00s.h>

#define TMP112_ADDR  0x49

i2c_rtc_m41t00s rtc;
hd44780_I2Cexp lcd;

const int LCD_ROWS = 4;
const int LCD_COLS = 20;
char dateString[20];
char timeString[20];

// the setup function runs once when you press reset or power the board
void setup() {

	// initialize LCD with number of columns and rows: 
	if (lcd.begin(LCD_COLS, LCD_ROWS))
	{
		// begin() failed so blink the onboard LED if possible

		fatalError(1); // this never returns
	}

	lcd.print("Hello, World!");
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
		sprintf(dateString, " %4u-%02u-%02u ", 2000 + rtc.time.year, rtc.time.month, rtc.time.day);
		lcd.setCursor(0, 0);
		lcd.print("DATE:");
		lcd.print(dateString);
		sprintf(timeString, " %02u:%02u:%02u ", rtc.time.hour, rtc.time.min, rtc.time.sec);
		lcd.setCursor(0, 1);
		lcd.print("TIME:");
		lcd.print(timeString);

	}

	else
	{
		lcd.setCursor(0, 0);
		lcd.print("I2C error 0x");
		lcd.print(u8Status, HEX);
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
	lcd.setCursor(0, 3);
	lcd.print("Internal: ");
	lcd.print(fTemp, 1);
	lcd.write(0xDF);
	lcd.print("C");

	delay(250);
}



void fatalError(int ecode)
{
	hd44780::fatalError(ecode); // does not return
}
