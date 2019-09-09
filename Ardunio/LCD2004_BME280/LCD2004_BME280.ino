// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2016-07-02
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// This sketch is for LCDs with PCF8574 or MCP23008 chip based backpacks
// WARNING:
//	Use caution when using 3v only processors like arm and ESP8266 processors
//	when interfacing with 5v modules as not doing proper level shifting or
//  incorrectly hooking things up can damage the processor.
// 
// Sketch will print "Hello, World!" on top row of lcd
// and will print the amount of time since the Arduino has been reset
// on the second row.
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//
// NOTE:
//	If the sketch fails to produce the expected results, or blinks the LED,
//	run the included I2CexpDiag sketch to test the i2c signals and the LCD.

 // i2c expander i/o class header

#include <BME280I2C.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>


hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip
BME280I2C bme;
					// LCD geometry
const int LCD_ROWS = 4;
const int LCD_COLS = 20;
bool metric = true;

void setup()
{
	// initialize LCD with number of columns and rows: 
	if (lcd.begin(LCD_COLS, LCD_ROWS))
	{
		// begin() failed so blink the onboard LED if possible

		fatalError(1); // this never returns
	}
	while (!bme.begin())
	{
		lcd.print("Hello, World!");
	}
	delay(1000);
	// Print a message to the LCD
}

void loop()
{
	float temp(NAN), hum(NAN), pres(NAN);
	 uint8_t pressureUnit(1);  //unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
	 bme.read(pres, temp, hum, pressureUnit, metric);
	 float altitude = bme.alt(metric);

	lcd.setCursor(0, 0);
	lcd.print("Temp: ");
	lcd.print(temp, 1);
	lcd.print(" C");
	lcd.write(0xdf);
	lcd.setCursor(0, 1);
	lcd.print("Humidity: ");
	lcd.print(hum,0);
	lcd.print("% RH");
	lcd.setCursor(0, 2);
	lcd.print("Pressure: ");
	lcd.print(pres, 1);
	lcd.print(" hPa");
	lcd.setCursor(0, 3);
	lcd.print("Altitude: ");
	lcd.print(altitude, 0);
	lcd.print((metric ? "m" : "ft"));

	delay(500);
}


// fatalError() - loop & blink an error code
void fatalError(int ecode)
{
	hd44780::fatalError(ecode); // does not return
}
