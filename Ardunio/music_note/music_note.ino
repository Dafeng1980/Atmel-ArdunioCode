#include <Wire.h>
#define B0  31
#define C1  33
#define CS1 35
#define D1  37
#define DS1 39l
#define E1  41
#define F1  44
#define FS1 46
#define G1  49
#define GS1 52
#define A1  55
#define AS1 58
#define B1  62
#define C2  65
#define CS2 69
#define D2  73
#define DS2 78
#define E2  82
#define F2  87
#define FS2 93
#define G2  98
#define GS2 104
#define A2  110
#define AS2 117
#define B2  123
#define C3  131
#define CS3 139
#define D3  147
#define DS3 156
#define E3  165
#define F3  175
#define FS3 185
#define G3  196
#define GS3 208
#define A3  220
#define AS3 233
#define B3  247
#define C4  262
#define CS4 277
#define D4  294
#define DS4 311
#define E4  330
#define F4  349
#define FS4 370
#define G4  392
#define GS4 415
#define A4  440
#define AS4 466
#define B4  494
#define C5  523
#define CS5 554
#define D5  587
#define DS5 622
#define E5  659
#define F5  698
#define FS5 740
#define G5  784
#define GS5 831
#define A5  880
#define AS5 932
#define B5  988
#define C6  1047
#define CS6 1109
#define D6  1175
#define DS6 1245
#define E6  1319
#define F6  1397
#define FS6 1480
#define G6  1568
#define GS6 1661
#define A6  1760
#define AS6 1865
#define B6  1976
#define C7  2093
#define CS7 2217
#define D7  2349
#define DS7 2489
#define E7  2637
#define F7  2794
#define FS7 2960
#define G7  3136
#define GS7 3322
#define A7  3520
#define AS7 3729
#define B7  3951
#define C8  4186
#define CS8 4435
#define D8  4699
#define DS8 4978
#define SIXTEENTHNOTE 1
#define EIGHTHNOTE 2
#define DOTTEDEIGHTNOTE 3
#define QUARTERNOTE 4
#define DOTTEDQUARTERNOTE 6
#define HALFNOTE 8
#define DOTTEDHALFNOTE 12
#define WHOLENOTE 16

#define M24LC128_ADDR 0x50
#define I2C_NOSTOP 0

#define speakerPin 12

int tempo = 120;
const int notemusic[72] = { G5,E6,DS6,E6,DS6,E6,B5,D6,C6,
A3,E4,A4,C5,E5,A5,E3,E4,GS4,E5,GS5,B5,
A3,E4,A4,E5,E6,DS6,E6,DS6,E6,B5,D6,C6,
A3,E4,A4,C5,E5,A5,E3,E4,GS4,E5,GS5,B5,
A3,E4,A4,E5,C6,D6,C4,G4,C5,G5,F6,E6,
G3,G4,B4,F5,E6,D6,A3,E4,A4,E5,D6,C6,
E3,E4,A5
};
const int notemusic1[146] = { 
  C4, E4, G4, E4, G4, E4, C4, E4, G4, E4, G4, E4, B3, E4, G4, E4, G4, E4, B3, E4, G4, E4, G4, E4,
  A3, E4, G4, E4, G4, E4, A3, E4, G4, E4, G4, E4, B3, E4, G4, E4, G4, E4, B3, E4, G4, E4, G4, E4,
  C5, E4, G4, E4, G4, E4, C5, E4, G4, E4, G4, E4, B4, E4, G4, E4, G4, E4, B4, E4, G4, E4, G4, E4,
  A4, E4, G4, E4, G4, E4, A4, E4, G4, E4, G4, E4, B4, E4, G4, E4, G4, E4, B4, E4, G4, E4, G4, E4,
  C5, E5, G5, E4, G5, E5, C5, E5, G5, E4, G5, E5, B4, E5, G5, E4, G5, E5, B4, E5, G5, E4, G5, E5,
  A4, E5, G5, E4, G5, E5, A4, E5, G5, E4, G5, E5, B4, E5, G5, E4, G5, E5, B4, E5, G5, E4, G5, E5,
  C5
};

void setup() {
  Serial.begin(38400);
   Wire.begin();
   for (int i = 72; i<200; i++){
    uint16_t val = notemusic1[i];
    uint8_t dataL = val & 0xFF;
    uint8_t dataH = val >> 8;
   M24LC128writeByte(2*i,dataL);
   M24LC128writeByte((2*i+1),dataH);
   Serial.print("dataL:");
   Serial.println(dataL,DEC);
   Serial.print("dataH:");
   Serial.println(dataH,DEC);
   
   }

   Serial.print(" M24LC128 ");
   for (int j=80; j<90; j++){
    Serial.println(MusicReadEeprom(2*j),DEC);
    delay(100);
   }
  // put your setup code here, to run once:

}

void loop() {

for (int i=0; i < 160; i++){
  note(MusicReadEeprom(2*i), EIGHTHNOTE);
}
   rest(EIGHTHNOTE);
   noTone(speakerPin);
}

int MusicReadEeprom(uint16_t address){
  
    int data;

  //uint8_t data; // `data` will store the register data   
  Wire.beginTransmission(M24LC128_ADDR);         // Initialize the Tx buffer
  Wire.write((int)(address >> 8));                // Put slave register address in Tx buffer
  Wire.write((int)(address & 0xFF));                // Put slave register address in Tx buffer
  Wire.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
//  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
//  Wire.requestFrom(address, 1);  // Read one byte from slave register address 
  Wire.requestFrom(M24LC128_ADDR,2);   // Read one byte from slave register address 
  uint8_t dataL = Wire.read();
  uint8_t dataH = Wire.read(); // Fill Rx buffer with result
  data = dataH*256 + dataL;
  return data;                             // Return data read from slave register
}
void M24LC128writeByte(uint16_t address, uint8_t  data)
{
//  uint8_t data_address1, data_address2;
//  data_address1 = highByte(data_address);
//  data_address2 = lowByte(data_address);
  Wire.beginTransmission(M24LC128_ADDR);   // Initialize the Tx buffer
  Wire.write((int)(address >> 8));                // Put slave register address in Tx buffer
  Wire.write((int)(address & 0xFF));                // Put slave register address in Tx buffer
  Wire.write(data);                         // Put data in Tx buffer
  Wire.endTransmission();                   // Send the Tx buffer
  delay(10);
  Serial.print(address >> 8, DEC);
  Serial.print(address & 0xFF, DEC);
  Serial.println("Wire once");
}
void M24LC128writeBytes(uint16_t data_address, uint8_t count, uint8_t * dest)
{
  uint8_t data_address1, data_address2;
  data_address1 = highByte(data_address);
  data_address2 = lowByte(data_address);
  if (count > 64) {
    count = 64;
    Serial.print("Page count cannot be more than 128 bytes!");
  }

  Wire.beginTransmission(M24LC128_ADDR);   // Initialize the Tx buffer
  Wire.write(data_address1);                // Put slave register address in Tx buffer
  Wire.write(data_address2);                // Put slave register address in Tx buffer
  for (uint8_t i = 0; i < count; i++) {
    Wire.write(dest[i]);                      // Put data in Tx buffer
  }
  Wire.endTransmission();                   // Send the Tx buffer
}
uint8_t M24LC128readByte(uint16_t address)
{
  uint8_t data;

  //uint8_t data; // `data` will store the register data   
  Wire.beginTransmission(M24LC128_ADDR);         // Initialize the Tx buffer
  Wire.write((int)(address >> 8));                // Put slave register address in Tx buffer
  Wire.write((int)(address & 0xFF));                // Put slave register address in Tx buffer
  Wire.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
//  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
//  Wire.requestFrom(address, 1);  // Read one byte from slave register address 
  Wire.requestFrom(M24LC128_ADDR,1);   // Read one byte from slave register address 
  data = Wire.read();                      // Fill Rx buffer with result
  return data;                             // Return data read from slave register
}
void M24LC128readBytes(uint16_t data_address, int count, uint8_t * dest)
{
  uint8_t data_address1, data_address2;
  data_address1 = highByte(data_address);
  data_address2 = lowByte(data_address);
  Wire.beginTransmission(M24LC128_ADDR);      // Initialize the Tx buffer
  Wire.write(data_address1);                     // Put slave register address in Tx buffer
  Wire.write(data_address2);                     // Put slave register address in Tx buffer
  Wire.endTransmission(I2C_NOSTOP);         // Send the Tx buffer, but send a restart to keep connection alive
//  Wire.endTransmission(false);              // Send the Tx buffer, but send a restart to keep connection alive
  uint8_t i = 0;
  //        Wire.requestFrom(address, count);       // Read bytes from slave register address 
  Wire.requestFrom(M24LC128_ADDR,count);  // Read bytes from slave register address 
  while (Wire.available()) {
    dest[i++] = Wire.read();
  }                // Put read results in the Rx buffer
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
