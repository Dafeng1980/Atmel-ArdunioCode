
#include <LiquidCrystal.h>
#include <Wire.h>

#define M41T00_SECS       0x00
#define M41T00_MINS       0x01
#define M41T00_HRS        0x02
#define M41T00_DAYS       0x03
#define M41T00_DATES      0x04
#define M41T00_MNTH       0x05
#define M41T00_YEAR       0x06
#define M41T00_CAB_CN    0x07

#define M41T00_ADDRESS             0x68
#define TMP112_ADDRESS             0x49
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
  Wire.beginTransmission(TMP112_ADDRESS);
   Wire.write(0x01);
   Wire.write(0x60);
   Wire.write(0xA0);
   Wire.endTransmission();
 //writeByte(M41T00_ADDRESS,M41T00_SECS,0x80);
  //writeByte(M41T00_ADDRESS,M41T00_MINS,0x52);
 // writeByte(M41T00_ADDRESS,M41T00_HRS,0x16);
   //writeByte(M41T00_ADDRESS,M41T00_DAYS,0x07);
 // writeByte(M41T00_ADDRESS,M41T00_DATES,0x23);
   //writeByte(M41T00_ADDRESS,M41T00_MNTH,0x10);
   //writeByte(M41T00_ADDRESS,M41T00_YEAR,0x16);
  // writeByte(M41T00_ADDRESS,M41T00_SECS,0x00);
  delay(50);
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
}

void loop()
{
lcd.setCursor(0, 0);
  uint8_t day = (readByte(M41T00_ADDRESS, M41T00_DAYS) & 0x07);
    if(day == 1) lcd.print("Mon ");
    if(day == 2) lcd.print("Tue ");   
    if(day == 3) lcd.print("Wed ");
    if(day == 4) lcd.print("Thr ");
    if(day == 5) lcd.print("Fri ");   
    if(day == 6) lcd.print("Sat ");
    if(day == 7) lcd.print("Sun ");
     uint8_t month = (10*((readByte(M41T00_ADDRESS, M41T00_MNTH) & 0x10) >> 4) + (readByte(M41T00_ADDRESS, M41T00_MNTH) & 0x0F));
     uint8_t date = (10*((readByte(M41T00_ADDRESS, M41T00_DATES) & 0x30) >> 4) + (readByte(M41T00_ADDRESS, M41T00_DATES) & 0x0F));
     uint8_t year = (10*((readByte(M41T00_ADDRESS, M41T00_YEAR) & 0xF0) >> 4) + (readByte(M41T00_ADDRESS, M41T00_YEAR) & 0x0F));
     uint8_t century = (readByte(M41T00_ADDRESS, M41T00_HRS) & 0x40) >> 6;
     lcd.print(month); lcd.print("/"); 
     lcd.print(date); lcd.print("/2"); 
     lcd.print(century); lcd.print(year);
     
     lcd.setCursor(0, 1);
     uint8_t hour = (10*((readByte(M41T00_ADDRESS, M41T00_HRS) & 0x30) >> 4) + (readByte(M41T00_ADDRESS, M41T00_HRS) & 0x0F));
     uint8_t mins = (10*((readByte(M41T00_ADDRESS, M41T00_MINS) & 0x70) >> 4) + (readByte(M41T00_ADDRESS, M41T00_MINS) & 0x0F));
     uint8_t sec = (10*((readByte(M41T00_ADDRESS, M41T00_SECS) & 0x70) >> 4) + (readByte(M41T00_ADDRESS, M41T00_SECS) & 0x0F));
     lcd.print(hour); 
     if(mins < 10) lcd.print(":0"); else lcd.print(":"); lcd.print(mins); 
     if(sec < 10) lcd.print(":0"); else lcd.print(":"); lcd.print(sec);
    delay(200);
   unsigned data[2];
  Wire.beginTransmission(TMP112_ADDRESS);
  Wire.write(0X00);
  Wire.endTransmission();
  delay(200);
  
  Wire.requestFrom(TMP112_ADDRESS, 2 );
    if (Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  int temp  =  ((data[0] * 256) + data[1]) / 16;      // (msb << 8 |lsb) >> 4;
  if ( temp >=2048 ) {
    temp -= 4096;
  }
    float cTemp = temp * 0.0625;
     lcd.setCursor(10, 1);  // print the number of seconds since reset:
  // lcd.print(F("Temp: "));
  lcd.print(cTemp, 1);
  lcd.write(0xDF);
  lcd.print("C");
   delay(500);

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
