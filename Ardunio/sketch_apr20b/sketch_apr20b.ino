// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       newClock_M41T62.ino
    Created:  2019/6/29 9:23:56
    Author:     DAFENG\Dafeng
*/

#include <Wire.h>
#include "M41T62.h"
#include <ShiftDisplay.h>
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define SIXTEENTHNOTE 1
#define EIGHTHNOTE 2
#define DOTTEDEIGHTNOTE 3
#define QUARTERNOTE 4
#define DOTTEDQUARTERNOTE 6
#define HALFNOTE 8
#define DOTTEDHALFNOTE 12
#define WHOLENOTE 16

#define speakerPin 12
#define TMP100_ADDR  0x4A

RTC_M41T62 rtc;
ShiftDisplay display(COMMON_ANODE, 8);

int tempo = 120;
volatile bool buttonPressed;
volatile int key;

const int BUTTON_PIN = 2;
char dateString[30];
//char timeString[20];
char tempString[5];
int st, n;



void buttonPressInterrupt() {
  
  buttonPressed = true;
  noSleep();
}

void setup()
{
  uint8_t k;
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  pinMode(BUTTON_PIN, INPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(0, OUTPUT);
  // attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressInterrupt, FALLING);
  buttonPressed = false;

  Serial.begin(38400);
  rtc.begin();
  rtc.checkFlags();
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  initTmp100();
  // delay(50);
  sleepMode(SLEEP_POWER_DOWN);
  

  printTime();
  Serial.println("");
  // Serial.println(F("S [S]tart"));
  // Serial.println(F("P sto[P]"));
  Serial.println(F("T set [T]ime"));
  Serial.println(F("A set [A]larm time"));
  Serial.println("");
  Serial.println(F("Choose a menu item:"));
  Serial.println(F("-------------------"));

  ledlight();

  if (Serial.available() > 0)
  {
    k = Serial.read();
    switch (k)
    {
    //case 'S':
    //case 's':
    //  rtc.setStop(1);
    //  break;

    //case 'P':
    //case 'p':
    //  rtc.setStop(0);
    //  break;
    case 'T':
    case 't':
      setTime();
      break;
    case 'A':
    case 'a':
      setAlarmTime();
    default:
      break;
    }
    delay(10);
    Serial.flush();
  }
  
  // rtc.alarmRepeat(4);  // set alarm repeat mode (once per year)
  Serial.print(F("Repeat Mode Set: "));
  Serial.println(rtc.alarmRepeat());
  rtc.alarmEnable(1);
  rtc.printAllBits();
  //displayDate();
  displayAll();
  key = 0;
  n = 0;
}


void loop()
{
  if (buttonPressed) {
    detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));
    //displayDate();
    displayAll();
    key = 0;
    n = 0;
    /*Serial.println(F("bottonPressed ture"));*/
    buttonPressed = false;
  }
  switch (key)
  {
  case 0 :
    displayTime();
      break;
  case 1 :
    displayTimeA();
    break;
  case 2 :
    displayDateA();
    break;
  case 3 :
    displayTemp();
    //key = 0;
    break;
  case 4 :
    displayAll();
  default:
    key = 0;
    break;
  }

  if (n == 900)
  {
    //displayTemp();
    displayAll();
    n = 0;
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressInterrupt, FALLING);
    delay(10);
    sleep();
    //if (st == 5)
    //{
    //  n = 0;
    //  st = 0;
    //  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressInterrupt, FALLING);
    //  delay(10);
    //  sleep();
    //}
    //st++;
    //n = 0;

  }
  n++;
  checkButton();
}

void initTmp100() {
  Wire.beginTransmission(TMP100_ADDR);
  Wire.write(0x01);
  Wire.write(0x31);  // 12 bits Resolution(320ms); in Shutdown Mode
  Wire.endTransmission();
}

void displayTime() {
  DateTime now = rtc.now();
  sprintf(dateString, "%02u %02u %02u", now.hour(), now.minute(), now.second());
  display.set(dateString);
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

void displayTimeA() {
  DateTime now = rtc.now();
  sprintf(dateString, "%02u __ %02u", now.hour(), now.minute());
  display.set(dateString);
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

void displayDateA() {
  DateTime now = rtc.now();
  sprintf(dateString, " %02u--%02u ", now.month(), now.day());
  display.set(dateString);
  display.show(1000);
}

void displayAll() {
  uint8_t data[2];
  Wire.beginTransmission(TMP100_ADDR);
  Wire.write(0x01);
  Wire.endTransmission();
  Wire.requestFrom(TMP100_ADDR, 1);
  uint8_t cros = Wire.read();
  bitWrite(cros, 6, 1);               //One-shot Temperature

  Wire.beginTransmission(TMP100_ADDR);
  Wire.write(0x01);
  Wire.write(cros);
  Wire.endTransmission();
  delay(320);

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
  DateTime time = rtc.now();
  display.show(100);
  // buttonPressed = false;
  sprintf(dateString, " %4u-%02u-%02u  %02u-%02u-%02u  %02u#%01uC ", time.year(), time.month(), time.day(),time.hour(), time.minute(), time.second(),fTemp, dot);
  //sprintf(timeString, " %02u %02u %02u ", time.hour(), time.minute(), time.second());
  String condition = dateString;

  condition = "        " + condition;
  while (condition.length() > 0) {
    display.show(condition, 650, ALIGN_LEFT);
    condition.remove(0, 1);
  }
}

void printTime()
{
  DateTime time = rtc.now();

  sprintf(dateString, "%4u-%02u-%02u %02u:%02u:%02u .",
    time.year(), time.month(), time.day(), time.hour(),
    time.minute(), time.second());
  Serial.println(dateString);
}

void setTime()
{
  uint8_t x, y;
  char date[11], time[8];
  uint8_t sec, min, hour, day, month;
  uint16_t year;
  Serial.println(F("Enter time format"));
  Serial.println(F("yyyy-mm-dd hh:mm:ss"));

  while (Serial.available() < 19)
  {
  }
  x = Serial.read(); // discard digit
  x = Serial.read(); // discard digit
  x = Serial.read(); // year: tens digit
  y = Serial.read(); // year: ones digit
  year = (2000 + 10 * (x - '0') + (y - '0'));

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
sec = 10 * (x - '0') + (y - '0');
delay(10);
Serial.flush();
rtc.adjust(DateTime(year, month, day, hour, min, sec));
Serial.println(F("Set Successful"));
}

void setAlarmTime() {
  uint8_t x, y;
  uint8_t sec, min, hour, day, month, mode;
  DateTime now = rtc.now();
  month = now.month();
  day = now.day();
  sec = 0;
  
  Serial.println(F("Enter alarm time format (mode is 1 to 6, 4 is per day)"));
  Serial.println(F("hh:mm mode"));

  while (Serial.available() < 8)
  {
  }
  x = Serial.read(); // hour: tens digit
  y = Serial.read(); // hour: ones digit
  hour = 10 * (x - '0') + (y - '0');

  x = Serial.read(); // discard spacer
  x = Serial.read(); // min: tens digit
  y = Serial.read(); // min: ones digit
  min = 10 * (x - '0') + (y - '0');
  x = Serial.read(); //  discard spacer
  y = Serial.read(); // alarmRepeat mode
  mode = y - '0';
  delay(10);
  Serial.flush();
  DateTime alarmTime(2019, month, day, hour, min, sec);
  rtc.alarmSet(alarmTime);
  rtc.alarmRepeat(mode);

  Serial.println(F("Set Successful"));
  //rtc.alarmRepeat(4);// set alarm repeat mode (once per day)
  //DateTime alarmTime(2019, month, day, hour, min, sec);
  // printTimeo(alarmTime);

  // rtc.alarmEnable(1);
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

void displayTemp() {
  uint8_t data[2];
  Wire.beginTransmission(TMP100_ADDR);
  Wire.write(0x01);
  Wire.endTransmission(); 
    Wire.requestFrom(TMP100_ADDR, 1);
    uint8_t cros = Wire.read();
  bitWrite(cros, 6, 1);               //One-shot Temperature
  
  Wire.beginTransmission(TMP100_ADDR);
    Wire.write(0x01);
  Wire.write(cros);
  Wire.endTransmission();
  delay(320);

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
  sprintf(tempString, "%02u#%01uC", fTemp, dot);
  display.show(tempString, 3000, ALIGN_CENTER);
  String Null = "        ";
  display.set(Null);
  display.show();
}

void checkButton() {
  
  if (digitalRead(BUTTON_PIN) == 0) {
    delay(10);
    if (digitalRead(BUTTON_PIN) == 0);
    {
      //rtc.printAllBits();
      if (rtc.checkFlags()) {

        //rtc.printAllBits();
        // delay(50);
        //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressInterrupt, FALLING);

        playMusic();
        // rtc.alarmEnable(1);
        //detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));
        key = 0;
      }
      else
      key++;
    }
  }
  
}
void ledlight() {
  for (int i = 1; i < 10; i++) {
    digitalWrite(0, HIGH);
    delay(300);
    digitalWrite(0, LOW);
    delay(300);

  }
 }
void playMusic(){
  // Song goes here
  //note(NOTE_G5, HALFNOTE);

  //Measure 1
  note(NOTE_E6, EIGHTHNOTE);
  note(NOTE_DS6, EIGHTHNOTE);

  //Measure 2
  note(NOTE_E6, EIGHTHNOTE);
  note(NOTE_DS6, EIGHTHNOTE);
  note(NOTE_E6, EIGHTHNOTE);
  note(NOTE_B5, EIGHTHNOTE);
  note(NOTE_D6, EIGHTHNOTE);
  note(NOTE_C6, EIGHTHNOTE);

  //Measure 3
  note(NOTE_A3, EIGHTHNOTE);
  note(NOTE_E4, EIGHTHNOTE);
  note(NOTE_A4, EIGHTHNOTE);
  note(NOTE_C5, EIGHTHNOTE);
  note(NOTE_E5, EIGHTHNOTE);
  note(NOTE_A5, EIGHTHNOTE);

  //Measure 4
  note(NOTE_E3, EIGHTHNOTE);
  note(NOTE_E4, EIGHTHNOTE);
  note(NOTE_GS4, EIGHTHNOTE);
  note(NOTE_E5, EIGHTHNOTE);
  note(NOTE_GS5, EIGHTHNOTE);
  note(NOTE_B5, EIGHTHNOTE);

  //Measure 5
  note(NOTE_A3, EIGHTHNOTE);
  note(NOTE_E4, EIGHTHNOTE);
  note(NOTE_A4, EIGHTHNOTE);
  note(NOTE_E5, EIGHTHNOTE);
  note(NOTE_E6, EIGHTHNOTE);
  note(NOTE_DS6, EIGHTHNOTE);

  //Measure 6
  note(NOTE_E6, EIGHTHNOTE);
  note(NOTE_DS6, EIGHTHNOTE);
  note(NOTE_E6, EIGHTHNOTE);
  note(NOTE_B5, EIGHTHNOTE);
  note(NOTE_D6, EIGHTHNOTE);
  note(NOTE_C6, EIGHTHNOTE);

  //Measure 7
  note(NOTE_A3, EIGHTHNOTE);
  note(NOTE_E4, EIGHTHNOTE);
  note(NOTE_A4, EIGHTHNOTE);
  note(NOTE_C5, EIGHTHNOTE);
  note(NOTE_E5, EIGHTHNOTE);
  note(NOTE_A5, EIGHTHNOTE);

  //Measure 8
  note(NOTE_E3, EIGHTHNOTE);
  note(NOTE_E4, EIGHTHNOTE);
  note(NOTE_GS4, EIGHTHNOTE);
  note(NOTE_E5, EIGHTHNOTE);
  note(NOTE_GS5, EIGHTHNOTE);
  note(NOTE_B5, EIGHTHNOTE);

  //Measure 9
  note(NOTE_A3, EIGHTHNOTE);
  note(NOTE_E4, EIGHTHNOTE);
  note(NOTE_A4, EIGHTHNOTE);
  note(NOTE_E5, EIGHTHNOTE);
  note(NOTE_C6, EIGHTHNOTE);
  note(NOTE_D6, EIGHTHNOTE);

  //Measure 10
  note(NOTE_C4, EIGHTHNOTE);
  note(NOTE_G4, EIGHTHNOTE);
  note(NOTE_C5, EIGHTHNOTE);
  note(NOTE_G5, EIGHTHNOTE);
  note(NOTE_F6, EIGHTHNOTE);
  note(NOTE_E6, EIGHTHNOTE);

  //Measure 11
  note(NOTE_G3, EIGHTHNOTE);
  note(NOTE_G4, EIGHTHNOTE);
  note(NOTE_B4, EIGHTHNOTE);
  note(NOTE_F5, EIGHTHNOTE);
  note(NOTE_E6, EIGHTHNOTE);
  note(NOTE_D6, EIGHTHNOTE);

  //Measure 12
  note(NOTE_A3, EIGHTHNOTE);
  note(NOTE_E4, EIGHTHNOTE);
  note(NOTE_A4, EIGHTHNOTE);
  note(NOTE_E5, EIGHTHNOTE);
  note(NOTE_D6, EIGHTHNOTE);
  note(NOTE_C6, EIGHTHNOTE);

  //Measure 13
  note(NOTE_E3, EIGHTHNOTE);
  note(NOTE_E4, EIGHTHNOTE);
  note(NOTE_A5, EIGHTHNOTE);
  rest(EIGHTHNOTE);



  /////// KEEP ALL CODE BELOW UNCHANGED, CHANGE VARS ABOVE ////////
  noTone(speakerPin);
}
void spacedNote(int frequencyInHertz, int noteLength)
{
  tone(speakerPin, frequencyInHertz);
  float delayTime = (1000 / tempo) * (60 / 4) * noteLength;
  delay(delayTime - 50);
  noTone(speakerPin);
  delay(50);
}
void note(int frequencyInHertz, int noteLength)  //Code to take care of the note
{
  tone(speakerPin, frequencyInHertz);
  float delayTime = (1000 / tempo) * (60 / 4) * noteLength;
  delay(delayTime);
}
void rest(int restLength)
{
  noTone(speakerPin);
  float delayTime = (1000 / tempo) * (60 / 4) * restLength;
  delay(delayTime);
}
