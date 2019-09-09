/*
 Name:    M41T00_TMP102_24LC128.ino
 Created: 12/16/2017 10:52:16 PM
 Author:  Dafeng
*/
#include <Wire.h>
#include <i2c_rtc_m41t00s.h>
#include <ShiftDisplay.h>

#define TMP112_ADDR  0x49

i2c_rtc_m41t00s rtc;
ShiftDisplay display(COMMON_ANODE, 8);

const int BUTTON_PIN = 2; 
char dateString[20];
char timeString[20];
char tempString[5];
  uint8_t n;
  volatile bool buttonPressed;

  void buttonPressInterrupt() {
  buttonPressed = true;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressInterrupt, FALLING);
  buttonPressed = false;
  rtc.begin();
  Wire.beginTransmission(TMP112_ADDR);
  Wire.write(0x01);
  Wire.write(0x60);
  Wire.write(0xA0);
  Wire.endTransmission();
  delay(50);
  
}

void loop() {

  uint8_t u8Status = rtc.get();
  uint8_t data[2];
  if (!u8Status)
  {
    //display.show(rtc.time.sec, 1000);
   // sprintf(dateString, " %4u-%02u-%02u ", 2000 + rtc.time.year, rtc.time.month, rtc.time.day);
    sprintf(timeString, "%02u %02u %02u", rtc.time.hour, rtc.time.min, rtc.time.sec);
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
  n++;
  if (buttonPressed) {
      display.show(100);
      buttonPressed = false;
      sprintf(dateString, " %4u-%02u-%02u ", 2000 + rtc.time.year, rtc.time.month, rtc.time.day);
      String condition = dateString;
      condition = "        " + condition;
    while (condition.length() > 0) {
    display.show(condition, 650, ALIGN_LEFT);
    condition.remove(0,1);
      }

    }
if (n==30)
{
  Wire.beginTransmission(TMP112_ADDR);
  Wire.write(0X00);
  Wire.endTransmission();
  //delay(50);
  Wire.requestFrom(TMP112_ADDR, 2);
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
  display.show(tempString, 5000, ALIGN_CENTER);
  n=0;
}

}
