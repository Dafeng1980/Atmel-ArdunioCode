// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       M41T62_Clock.ino
    Created:	2019/4/20 8:55:07
    Author:     DESKTOP-JI3C1IO\Dafeng
*/

// Define User Types below here or use a .h file

#include <Wire.h>
#include "M41T62.h"
#include <ShiftDisplay.h>
// Define Function Prototypes that use User Types below here or use a .h file
#define TMP100_ADDR  0x4A
RTC_M41T62 rtc;
ShiftDisplay display(COMMON_ANODE, 8);
// Define Functions below here or use other .ino or cpp files
const int BUTTON_PIN = 2;
char dateString[35];
char timeString[20];
char tempString[5];
uint8_t n, st;

volatile bool buttonPressed;

void buttonPressInterrupt() {
	buttonPressed = true;
	noSleep();
}

void setup()
{
#ifdef AVR
	Wire.begin();
#else
	Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressInterrupt, FALLING);
	buttonPressed = false;

	Serial.begin(38400);
	rtc.begin();
	// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	Wire.beginTransmission(TMP100_ADDR);
	Wire.write(0x01);
	Wire.write(0x30);
	Wire.endTransmission();
	delay(50);
	sleepMode(SLEEP_POWER_DOWN);
	displayDate();
}

void loop()
{
	uint8_t data[2];
	uint8_t k;
	if (buttonPressed) {
		displayDate();
		buttonPressed = false;
	}
	
	displayTime();

	n++;

	if (n == 30)
	{
		Wire.beginTransmission(TMP100_ADDR);
		Wire.write(0X00);
		Wire.endTransmission();
		Wire.requestFrom(TMP100_ADDR, 2);

		if (Wire.available() == 2) {
			data[0] = Wire.read();
			data[1] = Wire.read();
		}
		int temp1 = ((data[0] * 256) + data[1]) / 16;      // (msb << 8 |lsb) >> 4;
		if (temp1 >= 2048) {
			temp1 -= 4096;
		}
		int dot = (temp1 & 0x00F)*0.625;
		int fTemp = temp1 * 0.0625;
		sprintf(tempString, "%02u %01uC", fTemp, dot);
		display.show(tempString, 8000, ALIGN_CENTER);
		
		if (st == 6)
		{
			n = 0;
			st = 0;
			delay(10);
			sleep();
		}
		st++;
		n = 0;

	}
	if (Serial.available() > 0)
	{
		printTime();
		Serial.println("");
		Serial.println("S [S]tart");
		Serial.println("P sto[P]");
		Serial.println("T set Time");
		Serial.println("D [D]isplay time");
		Serial.println("");
		Serial.println("Choose a menu item:");
		Serial.println("--");

		k = Serial.read();
		switch (k)
		{
		case 'S':
		case 's':
			rtc.setStop(1);
			break;

		case 'P':
		case 'p':
			rtc.setStop(0);
			break;
		case 'T':
		case 't':
			setTime();
			break;
		case 'D':
		case 'd':
		default:
			break;
		}
		delay(10);
		Serial.flush();
	}

}

void printTime()
{
	DateTime time = rtc.now();

		sprintf(dateString, "%4u-%02u-%02u %02u:%02u:%02u .",
			time.year(), time.month(),time.day(), time.hour(),
			time.minute(),time.second() );
		Serial.println(dateString);
}

void setTime()
{
	uint8_t x, y;
	uint8_t sec, min, hour, day, month;
	uint16_t year;
	Serial.println("Enter time format");
	Serial.println("yyyy-mm-dd hh:mm:ss");

	while (Serial.available() < 19)
	{
	}
	x = Serial.read(); // discard digit
	x = Serial.read(); // discard digit
	x = Serial.read(); // year: tens digit
	y = Serial.read(); // year: ones digit
	year = (2000+10 * (x - '0') + (y - '0'));

	x = Serial.read(); // discard spacer
	x = Serial.read(); // month: tens digit
	y = Serial.read(); // month: ones digit
	month = 10 * (x - '0') + (y - '0');

	x = Serial.read(); // discard spacer
	x = Serial.read(); // day: tens digit
	y = Serial.read(); // day: ones digit
	day = 10 * (x - '0') + (y - '0');

	x = Serial.read(); // discard spacer
	x = Serial.read(); // hour: tens digit
	y = Serial.read(); // hour: ones digit
	hour = 10 * (x - '0') + (y - '0');

	x = Serial.read(); // discard spacer
	x = Serial.read(); // min: tens digit
	y = Serial.read(); // min: ones digit
	min = 10 * (x - '0') + (y - '0');

	x = Serial.read(); // discard spacer
	x = Serial.read(); // sec: tens digit
	y = Serial.read(); // sec: ones digit
	sec= 10 * (x - '0') + (y - '0');
	delay(10);
	Serial.flush();

	rtc.adjust(DateTime(year, month, day, hour, min, sec));
}

void displayDate()

{
	DateTime time = rtc.now();
	display.show(100);
	buttonPressed = false;
	sprintf(dateString, " %4u-%02u-%02u ", time.year(), time.month(), time.day());
	String condition = dateString;
	condition = "        " + condition;
	while (condition.length() > 0) {
		display.show(condition, 650, ALIGN_LEFT);
		condition.remove(0, 1);
	}
}

void displayTime() {
	DateTime now = rtc.now();
	sprintf(timeString, "%02u %02u %02u", now.hour(), now.minute(), now.second());
	display.set(timeString);
	display.setDot(0, true);
	display.setDot(1, true);
	display.setDot(2, true);
	display.setDot(3, true);
	display.setDot(4, true);
	display.setDot(5, true);
	display.show(500);
	display.setDot(0, false);
	display.setDot(1, false);
	display.setDot(2, false);
	display.setDot(3, false);
	display.setDot(4, false);
	display.setDot(5, false);
	display.show(500);
}