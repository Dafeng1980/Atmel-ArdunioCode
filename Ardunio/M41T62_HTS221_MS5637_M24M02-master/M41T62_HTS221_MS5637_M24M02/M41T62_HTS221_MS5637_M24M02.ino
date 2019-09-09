/* M41T62RTC_MS5637_HTS221_M24M02_t3 Basic Example Code
 by: Kris Winer
 date: January 25, 2015
 license: Beerware - Use this code however you'd like. If you 
 find it useful you can buy me a beer some time.
 
 Demonstrate M41T62 Real Time Clock operation including define registers, read time and date, set alarms,
 configure the interrupts, calibrate the clock with temperature compensation. Also read the pressure and
 temperature from the MS5637 altimeter, read the relative humidity from the HTS221 humidity sensor, andread from and write to the M24M02DFC 256 kbyte EEPROM.
 Sketch runs on the 3.3 V 8 MHz Pro Mini and the Teensy 3.1.
 
 This sketch is specifically for the Teensy 3.1 M41T62 RTC Add-On shield with the M41T62 RTC, an MS5637 pressure/temperature sensor, 
 anHTS221 humidity sensor, and an M24M02DFC 2Mkbit (64256 kByte) EEPROM as slave all connected via I2C. 
  
 This sketch uses SDA/SCL on pins 17/16, respectively, and it uses the Teensy 3.1-specific Wire library i2c_t3.h.
 The MS5637 is a simple but high resolution pressure sensor, which can be used in its high resolution
 mode but with power consumption of 20 microAmp, or in a lower resolution mode with power consumption of
 only 1 microAmp. The choice will depend on the application.
 
 SDA and SCL should have external pull-up resistors (to 3.3V).
 4k7 resistors are on the M41T62+MS5637+M24512DFM Mini Add-On board for Teensy 3.1.
 
 The M41T62+MS5637+HTS221+M24M02DFC board is so low power it can run off of digitalWrite on any GPIO pin. On the Teensy 3.1:
 
 Hardware setup:
 M41T62 Mini Add-On ------- Teensy 3.1
 VDD ----------------------  10 digitalWrite(HIGH)
 SDA ----------------------- 17
 SCL ----------------------- 16
 GND ----------------------  15 digitalWrite(LOW)
 IRQ------------------------  9
 SW-------------------------  8
 
 Note: The M41T62, MS5637, HTS221, and M245M02DFC are I2C devices and use the Teensy 3.1 i2c_t3.h Wire library. 
 Because the devices are not 5V tolerant, we are using a 3.3 V 8 MHz Pro Mini or a 3.3 V Teensy 3.1.
 */
//#include "Wire.h"   
#include <i2c_t3.h>
#include <SPI.h>
#include <FreqMeasure.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_PCD8544.h>
#include <Adafruit_SharpMem.h>

// any pins can be used
#define SCK 18
#define MOSI 19
#define SS 20

Adafruit_SharpMem display(SCK, MOSI, SS);

#define BLACK 0
#define WHITE 1

// M41T62 RTC register map
// http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00019860.pdf

#define M41T62_FRAC_SEC  0x00
#define M41T62_SECS      0x01
#define M41T62_MINS      0x02
#define M41T62_HRS       0x03
#define M41T62_DAY       0x04
#define M41T62_DATE      0x05
#define M41T62_MNTH      0x06
#define M41T62_YEAR      0x07
#define M41T62_CAL       0x08
#define M41T62_WDOG      0x09
#define M41T62_AL_MNTH   0x0A
#define M41T62_AL_DATE   0x0B
#define M41T62_AL_HR     0x0C
#define M41T62_AL_MIN    0x0D
#define M41T62_AL_SEC    0x0E
#define M41T62_FLAGS     0x0F

// See MS5637-02BA03 Low Voltage Barometric Pressure Sensor Data Sheet http://www.meas-spec.com/downloads/MS5637-02BA03.pdf
#define MS5637_RESET      0x1E
#define NS5637_CONVERT_D1 0x40
#define NS5637_CONVERT_D2 0x50
#define MS5637_ADC_READ   0x00


// See HTS221 Humidity Sensor:
// "Capacitive digital sensor for relative humidity and temperature"
// http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00116291.pdf

#define HTS221_WHO_AM_I          0x0F   // should be 0xBC
#define HTS221_AV_CONF           0x10       
#define HTS221_CTRL_REG1         0x20
#define HTS221_CTRL_REG2         0x21
#define HTS221_CTRL_REG3         0x22
#define HTS221_STATUS_REG        0x27
#define HTS221_HUMIDITY_OUT_L    0x28
#define HTS221_HUMIDITY_OUT_H    0x29
#define HTS221_TEMP_OUT_L        0x2A
#define HTS221_TEMP_OUT_H        0x2B
#define HTS221_CALIB_0           0x30  // CALIB 0 - F are read only!
#define HTS221_CALIB_1           0x31
#define HTS221_CALIB_2           0x32
#define HTS221_CALIB_3           0x33
#define HTS221_CALIB_4           0x34
#define HTS221_CALIB_5           0x35
#define HTS221_CALIB_6           0x36
#define HTS221_CALIB_7           0x37
#define HTS221_CALIB_8           0x38
#define HTS221_CALIB_9           0x39
#define HTS221_CALIB_A           0x3A
#define HTS221_CALIB_B           0x3B
#define HTS221_CALIB_C           0x3C
#define HTS221_CALIB_D           0x3D
#define HTS221_CALIB_E           0x3E
#define HTS221_CALIB_F           0x3F

// Device addresses
#define M41T62_ADDRESS             0x68 // Address of M41T62 RTC
#define MS5637_ADDRESS             0x76 // Address of MS5637 pressure/temperature sensor
#define HTS221_ADDRESS             0x5F // Address of HTS221 Humidity sensor
#define M24M02DRC_1_DATA_ADDRESS   0x50 // Address of the first 1024 page M24M02DRC EEPROM data buffer, 2048 bits (256 8-bit bytes) per page
#define M24M02DRC_1_IDPAGE_ADDRESS 0x58 // Address of the single M24M02DRC lockable ID page of the first EEPROM

// MS5637 pressure sensor sample rates
#define ADC_256  0x00 // define pressure and temperature conversion rates
#define ADC_512  0x02
#define ADC_1024 0x04
#define ADC_2048 0x06
#define ADC_4096 0x08
#define ADC_8192 0x0A
#define ADC_D1   0x40
#define ADC_D2   0x50

// HTS221 temperature averaging
enum avgT {
  avgT_2 = 0,
  avgT_4,
  avgT_8,
  avgT_16,  // default
  avgT_32,
  avgT_64,
  avgT_128,
  avgT_256
};

// HTS221 humidity averaging
enum avgH {
  avgH_4 = 0,
  avgH_8,
  avgH_16,  // default
  avgH_32,
  avgH_64,
  avgH_128,
  avgH_256,
  avgH_512
};

// HTS221 humidity and temperature sample ODR
enum HTODR {
  oneshot = 0,
  ODR_1Hz,
  ODR_7Hz, 
  ODR_12_5Hz 
 
};

// Specify sensor full scale
uint8_t OSR  = ADC_8192;         // set pressure amd temperature oversample rate
uint8_t avgT = avgT_256;          // set temperature avergaging for HTS221
uint8_t avgH = avgH_512;          // set humidity avergaging for HTS221
uint8_t HTODR = ODR_1Hz;      // set HTS221 humidity and temperature sample rate

// Calibration data for HTS221 humidity and temperature
uint8_t H0_rH_x2, H1_rH_x2, T0_degC_x8, T1_degC_x8, T0_msb, T1_msb;
int16_t H0_T0_OUT, H1_T0_OUT, T0_OUT, T1_OUT;

int myLed     = 13;    // LED on the Teensy 3.1 
int IRQ       =  9;    // INT on the Teensy 3.1
int SW        =  8;    // Calibration out on the Teensy 3.1
int VIN       =  15;   // digitalWrite(HIGH) this pin for 3V3
int GND       =  10;   // digitalWrite(LOW) this pin for GND
uint32_t delt_t = 0, count = 0;

// MS5637 variables
uint16_t Pcal[8];         // calibration constants from MS5637 PROM registers
unsigned char nCRC;       // calculated check sum to ensure PROM integrity
uint32_t D1 = 0, D2 = 0;  // raw MS5637 pressure and temperature data
double dT, OFFSET, SENS, T2, OFFSET2, SENS2;  // First order and second order corrections for raw S5637 temperature and pressure data
double Temperature, Pressure; // stores MS5637 pressures sensor pressure and temperature
int16_t temperatureCount = 0, humidityCount = 0;  // variables to hold raw HTS221 temperature and humidity values
float HTS221_humidity, HTS221_temperature;

// variables for frequency determination
#define MainPeriod 800
long previousMillis = 0; // will store last time of cycle end
volatile unsigned long duration = 0; // accumulate pulse width
volatile unsigned int pulsecount = 0; 
volatile unsigned long previousMicros = 0;
float Freq, sumFreq, avgFreq;
uint8_t sumCount;

void setup()
{
  // Set up output pins, especially power pins with digitalWrite
  pinMode(IRQ, INPUT); 
  pinMode(SW, INPUT);
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
  pinMode(VIN, OUTPUT);
  digitalWrite(VIN, HIGH);
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);
 
  // Setup for Master mode, pins 16/17, external pullups, 400kHz for Teensy 3.1
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_16_17, I2C_PULLUP_EXT, I2C_RATE_400);
  delay(2000);
  Serial.begin(38400);
  
  attachInterrupt(SW, myinthandler, RISING);  // define interrupt for SW output of M41T62 RTC
 
  I2Cscan();  // Check for I2C devices on the I2C bus
  
/*  
  // Make sure there are enough bytes left on the page (a page is the collection of bytes with the
  // same XSadd and MSadd) for the number you are writing otherwise they will wrap to the start of the
  // page and overwrite whetver was there. This is why 256 (or less)-byte page write is so useful.
  M24M02DRCwriteBytes(M24M02DRC_2_DATA_ADDRESS | XSadd, MSadd, LSadd, 10, data); // write data starting at first byte of page MSadd
  // It takes a maximum of 10 ms for a read or write operation; the EEPROM won't respond until the operation is done
  // There are ways to speed this up by polling, consult the data sheet
  delay(10);
  
  // Lastly, write a page to EEPROM 1 and then read it out
  for (int i = 0; i < 256; i++) { // generate the "data"
  data[i] = (byte) i;
  }
  XSadd = 0x03; // 2-bits only, 0 - 3 for four quadrants
  MSadd = 0xFF; // 256 bits only, 0 - 255
  LSadd = 0x00;// page starts at LSadd = 0x00 and shouldn't be more than 256 bytes long
  M24M02DRCwriteBytes(M24M02DRC_1_DATA_ADDRESS | XSadd, MSadd, LSadd, 256, data); // write data starting at first byte of page XSadd << 8 | MSadd
  delay(100);
  // Read 256 byte page from EEPROM 1
  M24M02DRCreadBytes(M24M02DRC_1_DATA_ADDRESS | XSadd, MSadd, LSadd, 256, output);
  Serial.println("Reading page "); Serial.print(XSadd << 8 | MSadd); Serial.println(" from EEPROM 1");
  for (int i = 0; i < 16; i++) {
  Serial.println(" ");
  for (int j = 0; j < 16; j++) {
  Serial.print(output[i*16 + j], HEX); Serial.print(" ");
  }
 */ 
  
// Set up for data display
   display.begin(); // Initialize the display
  display.setTextSize(1); // Set text size to normal, 2 is twice normal etc.
  display.setTextColor(BLACK); // Set pixel color; 1 on the monochrome screen
  display.clearDisplay();   // clears the screen and buffer

// Start device display with ID of sensor
  display.setCursor(0, 0); display.print("M41T62 RTC");
  display.setCursor(0, 20); display.print("MS5637 PRESS");
  display.setCursor(0,40); display.print("HTS221 HUMIDY");
  display.setCursor(0,60); display.print("M24M02 EEPROM");
  display.refresh();
  delay(2000);

  // Reset the MS5637 pressure sensor
  MS5637Reset();
  delay(100);
  Serial.println("MS5637 pressure sensor reset...");
  // Read PROM data from MS5637 pressure sensor
  MS5637PromRead(Pcal);
  Serial.println(" "); Serial.println("PROM data read:");
  Serial.print("C0 = "); Serial.println(Pcal[0]);
  unsigned char refCRC = Pcal[0] >> 12;
  Serial.print("C1 = "); Serial.println(Pcal[1]);
  Serial.print("C2 = "); Serial.println(Pcal[2]);
  Serial.print("C3 = "); Serial.println(Pcal[3]);
  Serial.print("C4 = "); Serial.println(Pcal[4]);
  Serial.print("C5 = "); Serial.println(Pcal[5]);
  Serial.print("C6 = "); Serial.println(Pcal[6]);
  
  nCRC = MS5637checkCRC(Pcal);  //calculate checksum to ensure integrity of MS5637 calibration data
  Serial.println(" "); Serial.print("Checksum = "); Serial.print(nCRC); Serial.print(" , should be "); Serial.println(refCRC);  
  
  display.clearDisplay();
  display.setCursor(30,0); display.print("MS5637");
  display.setCursor(0,10); display.print("CRC is "); display.setCursor(50,10); display.print(nCRC);
  display.setCursor(0,20); display.print("Should be "); display.print(refCRC);
  display.refresh();
  delay(2000);  
  
  // Check WHO_AM_I register of HTS221
  uint8_t c = readByte(HTS221_ADDRESS, HTS221_WHO_AM_I);
  Serial.println(" "); Serial.print("HTS221 WHO_AM_I is 0x"); Serial.print(c, HEX); Serial.println(", should be 0xBC");
  display.clearDisplay();
  display.setCursor(30,0); display.print("HTS221");
  display.setCursor(0,10); display.print("WHOAMI is "); display.print(c, HEX);
  display.setCursor(0,20); display.print("Should be "); display.print("BC");
  display.refresh();
  delay(2000);  

  if( c == 0xBC) 
  {
    HTS221init();
    Serial.println("HTS221 initialized!");
    
  // Read calibrations data
  H0_rH_x2 = readByte(HTS221_ADDRESS, HTS221_CALIB_0);
  H1_rH_x2 = readByte(HTS221_ADDRESS, HTS221_CALIB_1);
  T0_degC_x8 = readByte(HTS221_ADDRESS, HTS221_CALIB_2);
  T1_degC_x8 = readByte(HTS221_ADDRESS, HTS221_CALIB_3);
  T0_msb = readByte(HTS221_ADDRESS, HTS221_CALIB_5) & 0x03;
  T1_msb = (readByte(HTS221_ADDRESS, HTS221_CALIB_5) & 0x0C) >> 2;
  H0_T0_OUT = (int16_t) ((int16_t) readByte(HTS221_ADDRESS, HTS221_CALIB_7) << 8 | readByte(HTS221_ADDRESS, HTS221_CALIB_6));
  H1_T0_OUT = (int16_t) ((int16_t) readByte(HTS221_ADDRESS, HTS221_CALIB_B) << 8 | readByte(HTS221_ADDRESS, HTS221_CALIB_A));
  T0_OUT = (int16_t) ((int16_t) readByte(HTS221_ADDRESS, HTS221_CALIB_D) << 8 | readByte(HTS221_ADDRESS, HTS221_CALIB_C));
  T1_OUT = (int16_t) ((int16_t) readByte(HTS221_ADDRESS, HTS221_CALIB_F) << 8 | readByte(HTS221_ADDRESS, HTS221_CALIB_E));
  Serial.println(" "); 
  Serial.println("HTS221 Calibration Coefficients");
  Serial.print("H0_rH_x2 = "); Serial.println(H0_rH_x2);
  Serial.print("H1_rH_x2 = "); Serial.println(H1_rH_x2);
  Serial.print("T0_degC_x8 = "); Serial.println(T0_degC_x8);
  Serial.print("T1_degC_x8 = "); Serial.println(T1_degC_x8);
  Serial.print("T0_msb = "); Serial.println(T0_msb);
  Serial.print("T1_msb = "); Serial.println(T1_msb);
  Serial.print("H0_T0_OUT = "); Serial.println(H0_T0_OUT );
  Serial.print("H1_T0_OUT = "); Serial.println(H1_T0_OUT);
  Serial.print("T0_OUT = "); Serial.println(T0_OUT);
  Serial.print("T1_OUT = "); Serial.println(T1_OUT);
  Serial.println(" "); 
  }
  else Serial.println(" HTS221 not functioning!");
  
/*
// Set the clock
  writeByte(M41T62_ADDRESS, M41T62_MINS, 0x13);
  writeByte(M41T62_ADDRESS, M41T62_HRS,  0x21);
  writeByte(M41T62_ADDRESS, M41T62_DAY,  0x07);
  writeByte(M41T62_ADDRESS, M41T62_DATE, 0x25);
  writeByte(M41T62_ADDRESS, M41T62_MNTH, 0x01);
  writeByte(M41T62_ADDRESS, M41T62_YEAR, 0x15);
 */
 
//Set the stop bit
//writeByte(M41T62_ADDRESS, M41T62_SECS, 0x80);
//Set the start bit
//writeByte(M41T62_ADDRESS, M41T62_SECS, 0x00);
//delay(1000);
//Set the square-wave output to 512 Hz
uint8_t d = readByte(M41T62_ADDRESS, M41T62_DAY);
writeByte(M41T62_ADDRESS, M41T62_DAY, (d & 0x07) | 0x60); // set SW to 512 Hz
// Enable the square-wave output
writeByte(M41T62_ADDRESS, M41T62_AL_MNTH, (readByte(M41T62_ADDRESS, M41T62_AL_MNTH) | 0x40));

}

void loop()
{  
  unsigned long currentMillis = millis();
  if( currentMillis - previousMillis >= MainPeriod)
  {
    previousMillis = currentMillis;
    unsigned long _duration = duration;
    unsigned long _pulsecount = pulsecount;
    duration = 0;
    pulsecount = 0;
    Freq = 1000000. / ((float) _duration);
    Freq *= _pulsecount;
    Serial.println("  ");
    Serial.println(currentMillis); Serial.println(Freq); Serial.println(_pulsecount); Serial.println(_duration);
  }
 //   sumFreq += Freq;
 //   sumCount++;
 //   if (sumCount > 9) {
 //     Freq = sumFreq/(float)sumCount;
 //     sumFreq = 0.;
 //     sumCount = 0;
 //   }
  
  // Serial print and/or display at 1 s rate independent of data rates
    delt_t = millis() - count;
    if (delt_t > 1000) { // update LCD once per second independent of read rate
   
    Serial.println(" ");
    
    // Read from M41T62
    uint8_t day = (readByte(M41T62_ADDRESS, M41T62_DAY) & 0x07);
    Serial.print("Day of week is: ");
    if(day == 1) Serial.println("Monday");
    if(day == 2) Serial.println("Tuesday");   
    if(day == 3) Serial.println("Wednesday");
    if(day == 4) Serial.println("Thursday");
    if(day == 5) Serial.println("Friday");   
    if(day == 6) Serial.println("Saturday");
    if(day == 7) Serial.println("Sunday");
    
     uint8_t hour = (10*((readByte(M41T62_ADDRESS, M41T62_HRS) & 0x30) >> 4) + (readByte(M41T62_ADDRESS, M41T62_HRS) & 0x0F));
     uint8_t min = (10*((readByte(M41T62_ADDRESS, M41T62_MINS) & 0x70) >> 4) + (readByte(M41T62_ADDRESS, M41T62_MINS) & 0x0F));
     uint8_t sec = (10*((readByte(M41T62_ADDRESS, M41T62_SECS) & 0x70) >> 4) + (readByte(M41T62_ADDRESS, M41T62_SECS) & 0x0F));
     Serial.print("Time is: "); Serial.print(hour); 
     if(min < 10) Serial.print(":0"); else Serial.print(":"); Serial.print(min); 
     if(sec < 10) Serial.print(":0"); else Serial.print(":"); Serial.println(sec);
 
     uint8_t month = (10*((readByte(M41T62_ADDRESS, M41T62_MNTH) & 0x10) >> 4) + (readByte(M41T62_ADDRESS, M41T62_MNTH) & 0x0F));
     uint8_t date = (10*((readByte(M41T62_ADDRESS, M41T62_DATE) & 0x30) >> 4) + (readByte(M41T62_ADDRESS, M41T62_DATE) & 0x0F));
     uint8_t year = (10*((readByte(M41T62_ADDRESS, M41T62_YEAR) & 0xF0) >> 4) + (readByte(M41T62_ADDRESS, M41T62_YEAR) & 0x0F));
     uint8_t century = (readByte(M41T62_ADDRESS, M41T62_MNTH) & 0xC0) >> 6;
     Serial.print("Date is: "); Serial.print(month); Serial.print("/"); Serial.print(date); Serial.print("/2"); Serial.print(century); Serial.println(year);
    

    D1 = MS5637Read(ADC_D1, OSR);  // get raw pressure value
    D2 = MS5637Read(ADC_D2, OSR);  // get raw temperature value
    dT = D2 - Pcal[5]*pow(2,8);    // calculate temperature difference from reference
    OFFSET = Pcal[2]*pow(2, 17) + dT*Pcal[4]/pow(2,6);
    SENS = Pcal[1]*pow(2,16) + dT*Pcal[3]/pow(2,7);
 
    Temperature = (2000 + (dT*Pcal[6])/pow(2, 23))/100;           // First-order Temperature in degrees Centigrade
//
// Second order corrections
    if(Temperature > 20) 
    {
      T2 = 5*dT*dT/pow(2, 38); // correction for high temperatures
      OFFSET2 = 0;
      SENS2 = 0;
    }
    if(Temperature < 20)                   // correction for low temperature
    {
      T2      = 3*dT*dT/pow(2, 33); 
      OFFSET2 = 61*(100*Temperature - 2000)*(100*Temperature - 2000)/16;
      SENS2   = 29*(100*Temperature - 2000)*(100*Temperature - 2000)/16;
    } 
    if(Temperature < -15)                      // correction for very low temperature
    {
      OFFSET2 = OFFSET2 + 17*(100*Temperature + 1500)*(100*Temperature + 1500);
      SENS2 = SENS2 + 9*(100*Temperature + 1500)*(100*Temperature + 1500);
    }
 // End of second order corrections
 //
     Temperature = Temperature - T2/100;
     OFFSET = OFFSET - OFFSET2;
     SENS = SENS - SENS2;
 
     Pressure = (((D1*SENS)/pow(2, 21) - OFFSET)/pow(2, 15))/100;  // Pressure in mbar or hPa

 //    float altitude = 145366.45*(1. - pow((Pressure/1013.25), 0.190284)); // pressure in atmospheres
     float altitude = 44330.8 - 4946.54*pow((Pressure*100.0), 0.1902632);
     
     Serial.println(" ");     
     Serial.print("MS5637 temperature value = "); Serial.print( (float)Temperature, 2); Serial.println(" C"); // temperature in degrees Celsius
     Serial.print("MS5637 temperature value = "); Serial.print(9.*(float) Temperature/5. + 32., 2); Serial.println(" F"); // temperature in degrees Fahrenheit
     Serial.print("MS5637 pressure value = "); Serial.print((float) Pressure, 2);  Serial.println(" mbar");// pressure in millibar
     Serial.print("Altitude = "); Serial.print(altitude*3.280834, 2); Serial.println(" feet");
     
     if(readByte(HTS221_ADDRESS, HTS221_STATUS_REG) & 0x02) {
       humidityCount = readHumidityData();
       HTS221_humidity = (((float)humidityCount - (float)H0_T0_OUT)/((float)H1_T0_OUT - (float)H0_T0_OUT))*((float)H1_rH_x2 - (float)H0_rH_x2)/2. + (float) H0_rH_x2/2.;
       Serial.println(" ");
       Serial.print("HTS221 Humidity = "); Serial.print(HTS221_humidity, 2); Serial.println(" %rH"); // relative humidity
    }
  
     if(readByte(HTS221_ADDRESS, HTS221_STATUS_REG) & 0x01) {
       temperatureCount = readTempData();
       uint16_t outT0 = ((uint16_t) T0_msb << 8) | T0_degC_x8;
       uint16_t outT1 = ((uint16_t) T1_msb << 8) | T1_degC_x8;
       HTS221_temperature = (((float)temperatureCount - (float)T0_OUT)/((float)T1_OUT - (float)T0_OUT))*((float)outT1 - (float)outT0)/8. + (float) outT0/8.;
       Serial.print("HTS221 Temperature = "); Serial.print(HTS221_temperature, 2); Serial.println(" C"); // temperature in degrees Celsius
       Serial.print("HTS221 Temperature = "); Serial.print(9.*HTS221_temperature/5. +32., 2); Serial.println(" F"); // temperature in degrees Fahrenheit
     }
     
    
     display.clearDisplay();
     display.setCursor(0,0); display.print("P "); display.print((float)Pressure); display.print(" mbar");
     display.setCursor(0,10); display.print("Alt "); display.print(altitude*3.280834, 1); display.println(" ft");
     display.setCursor(0,20); display.print("T "); display.print((float) Temperature, 1); display.print(" C "); display.print(9.*((float) Temperature)/5. +32., 1); display.println(" F");
     display.setCursor(0,30); display.print("rH "); display.print(HTS221_humidity); display.print(" %rH");
     display.setCursor(0,40); display.print("T "); display.print(HTS221_temperature, 1); display.print(" C "); display.print(9.*HTS221_temperature/5. +32., 1); display.println(" F");
     display.setCursor(0,50);
     if(day == 1) display.print("Mon ");
     if(day == 2) display.print("Tue ");   
     if(day == 3) display.print("Wed ");
     if(day == 4) display.print("Thu ");
     if(day == 5) display.print("Fri ");   
     if(day == 6) display.print("Sat ");
     if(day == 7) display.print("Sun ");
     display.print(hour); 
     if(min < 10) display.print(":0"); else display.print(":"); display.print(min); 
     if(sec < 10) display.print(":0"); else display.print(":"); display.println(sec);
     display.setCursor(0,60); display.print(month); display.print("/"); display.print(date); display.print("/2"); display.print(century); display.println(year);
     display.setCursor(0,70); display.print(Freq, 4); display.print(" Hz "); 
      
     uint8_t calib = (int)(1000000.*(Freq - 512.)/512.);  // calculate number of calibration bits
     if(calib > 0) calib /= 2; // if clock fast, subtract calib/2 counts in calibration counter to slow it down
     if(calib < 0) calib /= 4;  // if clock slow, add calib/4 counts in the calibration counter to speed it up
     // calibration register max is 0x1F = 31, sign is positive if bit 5 is set to 1
     if (calib >= -31 && calib < 0) writeByte(M41T62_ADDRESS, M41T62_CAL, 0x20 | calib);  // clock fast, add 4 ppm/count to slow a bit
     if (calib <= +31 && calib > 0) writeByte(M41T62_ADDRESS, M41T62_CAL, calib);          // clock slow, subtract 2 ppm/count to speed up a bit
     display.setCursor(0,80); display.print((int)(1000000.*(Freq - 512.)/512.)); display.print(" ppm "); 
     uint8_t calreg = readByte(M41T62_ADDRESS, M41T62_CAL);
     if (calreg <= 0x0F) {display.print("cal 0x0"); display.print(calreg, HEX);}
     if (calreg >  0x0F) {display.print("cal 0x");  display.print(calreg, HEX);}
     display.refresh();  

    Serial.println(" ");
    digitalWrite(myLed, HIGH);
    delay(1);
    digitalWrite(myLed, LOW);
    count = millis();   
    }

}

//===================================================================================================================
//====== Set of useful function to access EEPROM, RTC, pressure, humidity, and temperature data
//===================================================================================================================

void myinthandler()
{
  unsigned long currentMicros = micros();
  duration += currentMicros - previousMicros;
  previousMicros = currentMicros;
  pulsecount++;
}

int16_t readTempData()
{
  uint8_t rawData[2];  // register data stored here
  readBytes(HTS221_ADDRESS, HTS221_TEMP_OUT_L | 0x80, 2, &rawData[0]);  // Read the two raw data registers sequentially into data array 
//  rawData[1] = 0x00;
  return ((int16_t)rawData[1] << 8) | rawData[0] ;  // Turn the MSB and LSB into a 16-bit value
}

int16_t readHumidityData()
{
  uint8_t rawData[2];  // register data stored here
  readBytes(HTS221_ADDRESS, HTS221_HUMIDITY_OUT_L | 0x80, 2, &rawData[0]);  // Read the two raw data registers sequentially into data array 
//  rawData[1] = 0x00;
  return ((int16_t)rawData[1] << 8) | rawData[0] ;  // Turn the MSB and LSB into a 16-bit value
}

// I2C communication with the M24M02DRC EEPROM is a little different from I2C communication with the usual motion sensor
// since the address is defined by two bytes
void M24M02DRCwriteByte(uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint8_t data)
{
Wire.beginTransmission(device_address); // Initialize the Tx buffer
Wire.write(data_address1); // Put slave register address in Tx buffer
Wire.write(data_address2); // Put slave register address in Tx buffer
Wire.write(data); // Put data in Tx buffer
Wire.endTransmission(); // Send the Tx buffer
}
void M24M02DRCwriteBytes(uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint16_t count, uint8_t * dest)
{
if(count > 256) {
count = 256;
Serial.print("Page count cannot be more than 256 bytes!");
}
Wire.beginTransmission(device_address); // Initialize the Tx buffer
Wire.write(data_address1); // Put slave register address in Tx buffer
Wire.write(data_address2); // Put slave register address in Tx buffer
for(uint16_t i=0; i < count; i++) {
Wire.write(dest[i]); // Put data in Tx buffer
}
Wire.endTransmission(); // Send the Tx buffer
}
uint8_t M24M02DRCreadByte(uint8_t device_address, uint8_t data_address1, uint8_t data_address2)
{
uint8_t data; // `data` will store the register data
Wire.beginTransmission(device_address); // Initialize the Tx buffer
Wire.write(data_address1); // Put slave register address in Tx buffer
Wire.write(data_address2); // Put slave register address in Tx buffer
Wire.endTransmission(I2C_NOSTOP); // Send the Tx buffer, but send a restart to keep connection alive
// Wire.endTransmission(false); // Send the Tx buffer, but send a restart to keep connection alive
// Wire.requestFrom(address, 1); // Read one byte from slave register address
Wire.requestFrom(device_address, (size_t) 1); // Read one byte from slave register address
data = Wire.read(); // Fill Rx buffer with result
return data; // Return data read from slave register
}
void M24M02DRCreadBytes(uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint16_t count, uint8_t * dest)
{
Wire.beginTransmission(device_address); // Initialize the Tx buffer
Wire.write(data_address1); // Put slave register address in Tx buffer
Wire.write(data_address2); // Put slave register address in Tx buffer
Wire.endTransmission(I2C_NOSTOP); // Send the Tx buffer, but send a restart to keep connection alive
// Wire.endTransmission(false); // Send the Tx buffer, but send a restart to keep connection alive
uint8_t i = 0;
// Wire.requestFrom(address, count); // Read bytes from slave register address
Wire.requestFrom(device_address, (size_t) count); // Read bytes from slave register address
while (Wire.available()) {
dest[i++] = Wire.read(); } // Put read results in the Rx buffer
}


// I2C communication with the M24512DFM EEPROM is a little different from I2C communication with the usual motion sensor
// since the address is defined by two bytes

        void M24512DFMwriteByte(uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint8_t  data)
{
	Wire.beginTransmission(device_address);   // Initialize the Tx buffer
	Wire.write(data_address1);                // Put slave register address in Tx buffer
	Wire.write(data_address2);                // Put slave register address in Tx buffer
	Wire.write(data);                         // Put data in Tx buffer
	Wire.endTransmission();                   // Send the Tx buffer
}


       void M24512DFMwriteBytes(uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint8_t count, uint8_t * dest)
{
	if(count > 128) {
        count = 128;
        Serial.print("Page count cannot be more than 128 bytes!");
        }
        
        Wire.beginTransmission(device_address);   // Initialize the Tx buffer
	Wire.write(data_address1);                // Put slave register address in Tx buffer
	Wire.write(data_address2);                // Put slave register address in Tx buffer
	for(uint8_t i=0; i < count; i++) {
	Wire.write(dest[i]);                      // Put data in Tx buffer
	}
        Wire.endTransmission();                   // Send the Tx buffer
}


        uint8_t M24512DFMreadByte(uint8_t device_address, uint8_t data_address1, uint8_t data_address2)
{
	uint8_t data; // `data` will store the register data	 
	Wire.beginTransmission(device_address);         // Initialize the Tx buffer
	Wire.write(data_address1);                // Put slave register address in Tx buffer
	Wire.write(data_address2);                // Put slave register address in Tx buffer
	Wire.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
//	Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
//	Wire.requestFrom(address, 1);  // Read one byte from slave register address 
	Wire.requestFrom(device_address, (size_t) 1);   // Read one byte from slave register address 
	data = Wire.read();                      // Fill Rx buffer with result
	return data;                             // Return data read from slave register
}

        void M24512DFMreadBytes(uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint8_t count, uint8_t * dest)
{  
	Wire.beginTransmission(device_address);   // Initialize the Tx buffer
	Wire.write(data_address1);                     // Put slave register address in Tx buffer
	Wire.write(data_address2);                     // Put slave register address in Tx buffer
	Wire.endTransmission(I2C_NOSTOP);         // Send the Tx buffer, but send a restart to keep connection alive
//	Wire.endTransmission(false);              // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
//        Wire.requestFrom(address, count);       // Read bytes from slave register address 
        Wire.requestFrom(device_address, (size_t) count);  // Read bytes from slave register address 
	while (Wire.available()) {
        dest[i++] = Wire.read(); }                // Put read results in the Rx buffer
}


void HTS221init()
{
  writeByte(HTS221_ADDRESS, HTS221_AV_CONF, avgT << 3 | avgH);  // set temperature and humidity averaging
  // power on HTS221 (bit 7) and enable block data update (bit 2)
  writeByte(HTS221_ADDRESS, HTS221_CTRL_REG1, 0x80 | 0x04 | HTODR);  // set temperature and humidity sample ODR

}


// I2C communication with the MS5637 is a little different from that with the MPU9250 and most other sensors
// For the MS5637, we write commands, and the MS5637 sends data in response, rather than directly reading
// MS5637 registers

        void MS5637Reset()
        {
        Wire.beginTransmission(MS5637_ADDRESS);  // Initialize the Tx buffer
	Wire.write(MS5637_RESET);                // Put reset command in Tx buffer
	Wire.endTransmission();                  // Send the Tx buffer
        }
        
        void MS5637PromRead(uint16_t * destination)
        {
        uint8_t data[2] = {0,0};
        for (uint8_t ii = 0; ii < 7; ii++) {
          Wire.beginTransmission(MS5637_ADDRESS);  // Initialize the Tx buffer
          Wire.write(0xA0 | ii << 1);              // Put PROM address in Tx buffer
          Wire.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
	  uint8_t i = 0;
          Wire.requestFrom(MS5637_ADDRESS, 2);   // Read two bytes from slave PROM address 
	  while (Wire.available()) {
          data[i++] = Wire.read(); }               // Put read results in the Rx buffer
          destination[ii] = (uint16_t) (((uint16_t) data[0] << 8) | data[1]); // construct PROM data for return to main program
        }
        }

        uint32_t MS5637Read(uint8_t CMD, uint8_t OSR)  // temperature data read
        {
        uint8_t data[3] = {0,0,0};
        Wire.beginTransmission(MS5637_ADDRESS);  // Initialize the Tx buffer
        Wire.write(CMD | OSR);                  // Put pressure conversion command in Tx buffer
        Wire.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
        
        switch (OSR)
        {
          case ADC_256: delay(1); break;  // delay for conversion to complete
          case ADC_512: delay(3); break;
          case ADC_1024: delay(4); break;
          case ADC_2048: delay(6); break;
          case ADC_4096: delay(10); break;
          case ADC_8192: delay(20); break;
        }
       
        Wire.beginTransmission(MS5637_ADDRESS);  // Initialize the Tx buffer
        Wire.write(0x00);                        // Put ADC read command in Tx buffer
        Wire.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
        Wire.requestFrom(MS5637_ADDRESS, 3);     // Read three bytes from slave PROM address 
	while (Wire.available()) {
        data[i++] = Wire.read(); }               // Put read results in the Rx buffer
        return (uint32_t) (((uint32_t) data[0] << 16) | (uint32_t) data[1] << 8 | data[2]); // construct PROM data for return to main program
        }



unsigned char MS5637checkCRC(uint16_t * n_prom)  // calculate checksum from PROM register contents
{
  int cnt;
  unsigned int n_rem = 0;
  unsigned char n_bit;
  
  n_prom[0] = ((n_prom[0]) & 0x0FFF);  // replace CRC byte by 0 for checksum calculation
  n_prom[7] = 0;
  for(cnt = 0; cnt < 16; cnt++)
  {
    if(cnt%2==1) n_rem ^= (unsigned short) ((n_prom[cnt>>1]) & 0x00FF);
    else         n_rem ^= (unsigned short)  (n_prom[cnt>>1]>>8);
    for(n_bit = 8; n_bit > 0; n_bit--)
    {
        if(n_rem & 0x8000)    n_rem = (n_rem<<1) ^ 0x3000;
        else                  n_rem = (n_rem<<1);
    }
  }
  n_rem = ((n_rem>>12) & 0x000F);
  return (n_rem ^ 0x00);
}

// simple function to scan for I2C devices on the bus
void I2Cscan() 
{
    // scan for i2c devices
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}


// I2C read/write functions for the HTS221 sensors

        void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
	Wire.beginTransmission(address);  // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.write(data);                 // Put data in Tx buffer
	Wire.endTransmission();           // Send the Tx buffer
}

        uint8_t readByte(uint8_t address, uint8_t subAddress)
{
	uint8_t data; // `data` will store the register data	 
	Wire.beginTransmission(address);         // Initialize the Tx buffer
	Wire.write(subAddress);	                 // Put slave register address in Tx buffer
	Wire.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
//	Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
//	Wire.requestFrom(address, 1);  // Read one byte from slave register address 
	Wire.requestFrom(address, (size_t) 1);   // Read one byte from slave register address 
	data = Wire.read();                      // Fill Rx buffer with result
	return data;                             // Return data read from slave register
}

        void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
	Wire.beginTransmission(address);   // Initialize the Tx buffer
	Wire.write(subAddress);            // Put slave register address in Tx buffer
	Wire.endTransmission(I2C_NOSTOP);  // Send the Tx buffer, but send a restart to keep connection alive
//	Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
//        Wire.requestFrom(address, count);  // Read bytes from slave register address 
        Wire.requestFrom(address, (size_t) count);  // Read bytes from slave register address 
	while (Wire.available()) {
        dest[i++] = Wire.read(); }         // Put read results in the Rx buffer
}
