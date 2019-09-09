
#include <LiquidCrystal.h>
#include <Wire.h>

#define F8563_CONTR1     0x00
#define F8563_CONTR2     0x01
#define F8563_SECS       0x02
#define F8563_MINS       0x03
#define F8563_HRS        0x04
#define F8563_DAYS       0x05
#define F8563_WEEKS      0x06
#define F8563_MNTH       0x07
#define F8563_YEAR       0x08
#define F8563_AL_MNTH    0x09
#define F8563_AL_HR      0x0A
#define F8563_AL_DATE    0x0B
#define F8563_AL_WEEK    0x0C
#define F8563_CLKOUT_CN  0x0D
#define F8563_TIMER_CN   0x0E
#define F8563_TIMER      0x0F
#define F8563_ADDRESS             0x51

// Connections:
// rs (LCD pin 4) to Arduino pin 12
// rw (LCD pin 5) to Arduino pin 11
// enable (LCD pin 6) to Arduino pin 10
// LCD pin 15 to Arduino pin 13
// LCD pins d4, d5, d6, d7 to Arduino pins 5, 4, 3, 2

LiquidCrystal lcd(26, 27, 28, 29, 30, 31);

int backLight = 22;    // pin 13 will control the backlight

void setup()
{

  lcd.begin(16, 2);
  Wire.begin();
  //delay(20);
  //writeByte(F8563_ADDRESS,F8563_CONTR1,0x08);
 //writeByte(F8563_ADDRESS,F8563_SECS,0x80);
  //writeByte(F8563_ADDRESS,F8563_MINS,0x25);
  //writeByte(F8563_ADDRESS,F8563_HRS,0x16);
  // writeByte(F8563_ADDRESS,F8563_DAY,0x07);
  //writeByte(F8563_ADDRESS,F8563_DATE,0x16);
   //writeByte(F8563_ADDRESS,F8563_MNTH,0x10);
   //writeByte(F8563_ADDRESS,F8563_YEAR,0x16);
   //writeByte(F8563_ADDRESS,F8563_CONTR1,0x00);
   delay(50);
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
}

void loop()
{
lcd.setCursor(0, 0);
  uint8_t day = (readByte(F8563_ADDRESS, F8563_WEEKS) & 0x07);
    if(day == 1) lcd.print("Mon ");
    if(day == 2) lcd.print("Tue ");   
    if(day == 3) lcd.print("Wed ");
    if(day == 4) lcd.print("Thr ");
    if(day == 5) lcd.print("Fri ");   
    if(day == 6) lcd.print("Sat ");
    if(day == 0) lcd.print("Sun ");
     uint8_t month = (10*((readByte(F8563_ADDRESS, F8563_MNTH) & 0x10) >> 4) + (readByte(F8563_ADDRESS, F8563_MNTH) & 0x0F));
     uint8_t date = (10*((readByte(F8563_ADDRESS, F8563_DAYS) & 0x30) >> 4) + (readByte(F8563_ADDRESS, F8563_DAYS) & 0x0F));
     uint8_t year = (10*((readByte(F8563_ADDRESS, F8563_YEAR) & 0xF0) >> 4) + (readByte(F8563_ADDRESS, F8563_YEAR) & 0x0F));
     uint8_t century = (readByte(F8563_ADDRESS, F8563_MNTH) & 0x80) >> 7;
     lcd.print(month); lcd.print("/"); 
     lcd.print(date); lcd.print("/2"); 
     lcd.print(century); lcd.print(year);
     
     lcd.setCursor(0, 1);
     uint8_t hour = (10*((readByte(F8563_ADDRESS, F8563_HRS) & 0x30) >> 4) + (readByte(F8563_ADDRESS, F8563_HRS) & 0x0F));
     uint8_t mins = (10*((readByte(F8563_ADDRESS, F8563_MINS) & 0x70) >> 4) + (readByte(F8563_ADDRESS, F8563_MINS) & 0x0F));
     uint8_t sec = (10*((readByte(F8563_ADDRESS, F8563_SECS) & 0x70) >> 4) + (readByte(F8563_ADDRESS, F8563_SECS) & 0x0F));
     lcd.print(hour); 
     if(mins < 10) lcd.print(":0"); else lcd.print(":"); lcd.print(mins); 
     if(sec < 10) lcd.print(":0"); else lcd.print(":"); lcd.print(sec);
  delay(1000);

}

 void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

 uint8_t readByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data;                              // `data` will store the register data   
  Wire.beginTransmission(address);         // Initialize the Tx buffer
  Wire.write(subAddress);                  // Put slave register address in Tx buffer
  Wire.endTransmission();        // Send the Tx buffer, but send a restart to keep connection alive
//  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
//  Wire.requestFrom(address, 1);  // Read one byte from slave register address 
  Wire.requestFrom(address, (size_t) 1);   // Read one byte from slave register address 
  data = Wire.read();                      // Fill Rx buffer with result
  return data;                             // Return data read from slave register
}
