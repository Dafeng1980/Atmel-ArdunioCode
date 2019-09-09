// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#define LM73_ADDR    0x48
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      3

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() {
   Wire.begin();
   Wire.beginTransmission(LM73_ADDR);
   Wire.write(0x01);
   Wire.write(0x60);
   Wire.endTransmission();
   delay(10);
   Wire.beginTransmission(LM73_ADDR);
   Wire.write(0x04);
   Wire.write(0x40);
   Wire.endTransmission();
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  unsigned data[2];
  Wire.beginTransmission(LM73_ADDR);
  Wire.write(0X00);
  Wire.endTransmission();
  delay(50);
  Wire.requestFrom(LM73_ADDR, 2 );
    if (Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  int temp  =  ((data[0] * 256) + data[1]) / 8;      // (msb << 8 |lsb) >> 3;
  if (temp >= 4096) {
    temp -= 8192;
  }
    float cTemp = temp * 0.0625; 
  Serial.print("Celsius value : ");
  Serial.print(cTemp, 1);
  Serial.println(" C");

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if (cTemp>=25)
    pixels.setPixelColor(0, pixels.Color(130,0,0)); // Moderately bright green color.
   if(cTemp>=23 && cTemp < 25)
   pixels.setPixelColor(2, pixels.Color(130,50,0));
   if(cTemp>=21 && cTemp < 23)
   pixels.setPixelColor(0, pixels.Color(130,80,30));
   if(cTemp>=19 && cTemp < 21)
   pixels.setPixelColor(2, pixels.Color(130,150,150));
    if(cTemp>=17 && cTemp < 19)
   pixels.setPixelColor(0, pixels.Color(35,35,30));
   if(cTemp>=15 && cTemp < 17)
   pixels.setPixelColor(2, pixels.Color(0,100,30));
   if(cTemp < 15)
   pixels.setPixelColor(0, pixels.Color(0,30,150)); 
   
    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
