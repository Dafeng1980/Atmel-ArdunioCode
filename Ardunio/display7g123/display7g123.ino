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
char dateString[35];
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
  Serial.begin(38400);
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
  uint8_t k;
  if (!u8Status)
  {
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
  if(Serial.available()>0)
  {
      printTime();
  Serial.println("");
  Serial.println("S [S]tart");
  Serial.println("P sto[P]");
  Serial.println("F clear [F]ault");
  Serial.println("T set [T]ime");
  Serial.println("C set [C]al");
  Serial.println("D [D]isplay time");
  Serial.println("");
  Serial.println("Choose a menu item:");
  Serial.println("--");

  k = Serial.read();
   switch(k)
  {
    case 'S':
    case 's':
      u8Status = rtc.start();
      break;

    case 'P':
    case 'p':
      u8Status = rtc.stop();
      break;

    case 'F':
    case 'f':
      u8Status = rtc.clearFault();
      break;

    case 'T':
    case 't':
      setTime();
      break;

    case 'C':
    case 'c':
      setCal();
      break;

    case 'D':
    case 'd':
    default:
      break;
  }
  delay(10);
  Serial.flush();
  }
  
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

void printTime()
{
  uint8_t u8Status = rtc.get();

  if (!u8Status)
  {
    sprintf(dateString, "%4u-%02u-%02u %02u:%02u:%02u (DOW: %u, CAL: 0x%02x).",
      2000 + rtc.time.year, rtc.time.month, rtc.time.day, rtc.time.hour,
        rtc.time.min, rtc.time.sec, rtc.time.dow, rtc.time.cal);
    Serial.println(dateString);
  }
  else
  {
    Serial.print(u8Status, HEX);
  }
}
void setTime()
{
  uint8_t x, y, u8Status;
  i2c_rtc_m41t00s::time_t newtime;

  Serial.println("Enter time format (dow is Sun, Mon, ... Sat):");
  Serial.println("yyyy-mm-dd hh:mm:ss dow");
  while (Serial.available() < 22)
  {
  }

  x = Serial.read(); // discard digit
  x = Serial.read(); // discard digit
  x = Serial.read(); // year: tens digit
  y = Serial.read(); // year: ones digit
  newtime.year = 10 * (x - '0') + (y - '0');

  x = Serial.read(); // discard spacer
  x = Serial.read(); // month: tens digit
  y = Serial.read(); // month: ones digit
  newtime.month = 10 * (x - '0') + (y - '0');

  x = Serial.read(); // discard spacer
  x = Serial.read(); // day: tens digit
  y = Serial.read(); // day: ones digit
  newtime.day = 10 * (x - '0') + (y - '0');

  x = Serial.read(); // discard spacer
  x = Serial.read(); // hour: tens digit
  y = Serial.read(); // hour: ones digit
  newtime.hour = 10 * (x - '0') + (y - '0');

  x = Serial.read(); // discard spacer
  x = Serial.read(); // min: tens digit
  y = Serial.read(); // min: ones digit
  newtime.min = 10 * (x - '0') + (y - '0');

  x = Serial.read(); // discard spacer
  x = Serial.read(); // sec: tens digit
  y = Serial.read(); // sec: ones digit
  newtime.sec = 10 * (x - '0') + (y - '0');

  x = Serial.read(); // discard spacer
  x = Serial.read(); // dow: 1st character
  y = Serial.read(); // dow: 2nd character

  switch(x)
  {
    case 'S':
    case 's':
      if (y == 'U' or y == 'u')
      {
        newtime.dow = 1;
      }

      if (y == 'A' or y == 'a')
      {
        newtime.dow = 7;
      }
      break;

    case 'M':
    case 'm':
      newtime.dow = 2;
      break;

    case 'T':
    case 't':
      if (y == 'U' or y == 'u')
      {
        newtime.dow = 3;
      }

      if (y == 'H' or y == 'h')
      {
        newtime.dow = 5;
      }
      break;

    case 'W':
    case 'w':
      newtime.dow = 4;
      break;

    case 'F':
    case 'f':
      newtime.dow = 6;
      break;
  }

  Serial.flush();

  u8Status = rtc.set(&newtime);
}


void setCal()
{
  uint8_t x, y, u8Status;

  Serial.println("En Val InHex(skip 0x):");
  while (Serial.available() < 2)
  {
  }

  x = Serial.read(); // cal: upper nibble
  if (BOUND(x, '0', '9'))
  {
    x -= '0';
  }
  else if (BOUND(x, 'A', 'F'))
  {
    x -= ('A' - 10);
  }
  else if (BOUND(x, 'a', 'f'))
  {
    x -= ('a' - 10);
  }
  else
  {
    x = 0;
  }

  y = Serial.read(); // cal: lower nibble
  if (BOUND(y, '0', '9'))
  {
    y -= '0';
  }
  else if (BOUND(y, 'A', 'F'))
  {
    y -= ('A' - 10);
  }
  else if (BOUND(y, 'a', 'f'))
  {
    y -= ('a' - 10);
  }
  else
  {
    y = 0;
  }

  u8Status = rtc.setCalibration((x << 4) + y);
 
}
