
#include <Arduino.h>
#include <U8g2_Arduino-master\src\U8g2lib.h>

#include <BME280I2C.h>
#include <Wire.h>  
BME280I2C bme;  // Needed for legacy versions of Arduino.

U8G2_SH1106_128X64_VCOMH0_F_HW_I2C u8g2(U8G2_R0);

//#define SERIAL_BAUD 38400


bool metric = true;


void setup() {
	u8g2.begin();
	u8g2.enableUTF8Print();
	while (!bme.begin())
	{
		u8g2.clearBuffer();
		u8g2.setFont(u8g2_font_ncenB08_tr);  // use chinese2 for all the glyphs of "ƒ„∫√ ¿ΩÁ"
		u8g2.setFontDirection(0);
		u8g2.setCursor(0, 15);
		u8g2.println("Could not find BME280I2C sensor!");
		u8g2.sendBuffer();
		delay(1000);
	}  
}

void loop() {
	float temp(NAN), hum(NAN), pres(NAN);
	 uint8_t pressureUnit(3);   // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
	bme.read(pres, temp, hum, pressureUnit, metric);  
	 float altitude = bme.alt(metric);                                // Parameters: (float& pressure, float& temp, float& humidity, bool hPa = true, bool celsius = false)

													
	u8g2.setFont(u8g2_font_ncenB08_tr);
	u8g2.setFontDirection(0);
	u8g2.clearBuffer();
	u8g2.setCursor(0, 10);
	u8g2.print("Temp: ");
	u8g2.print(temp,1);
	u8g2.print(" C");
	u8g2.setCursor(0, 25);
	u8g2.print("Humidity: ");
	u8g2.print(hum,0);
    u8g2.print("% RH");
	u8g2.setCursor(0, 40);
	u8g2.print("Pressure: ");
	u8g2.print(pres);
	u8g2.print(" atm");
	u8g2.setCursor(0, 55);	
	u8g2.print("Altitude: ");
	u8g2.print(altitude,1);
	u8g2.print((metric ? "m" : "ft"));
	u8g2.sendBuffer();
	delay(500);
}
