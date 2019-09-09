  /*  
  OpenMQTTGateway Addon  - ESP8266 or Arduino program for home automation 

   Act as a wifi or ethernet gateway between your 433mhz/infrared IR signal  and a MQTT broker 
   Send and receiving command by MQTT
 
   This is the Light Meter Addon:
   - Measures ambient Light Intensity in Lux (lx), Foot Candela (ftcd) and Watt/m^2 (wattsm2)
   - Required Hardware Module: BH1750

   Connection Schemata:
   --------------------

   BH1750 ------> Arduino Uno ----------> ESP8266
   ==============================================
   Vcc ---------> 5V -------------------> Vu (5V)
   GND ---------> GND ------------------> GND
   SCL ---------> Pin A5 ---------------> D1
   SDA ---------> Pin A4 ---------------> D2
   ADD ---------> N/C (Not Connected) --> N/C (Not Connected)
 
    Copyright: (c) Hans-Juergen Dinges
  
    This file is part of OpenMQTTGateway.
    
    OpenMQTTGateway is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenMQTTGateway is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef ZsensorBH1750
#include "math.h" // Library for trig and exponential functions
#include "Wire.h" // Library for communication with I2C / TWI devices

void setupZsensorBH1750()
{
  Wire.begin();
  Wire.beginTransmission(BH1750_i2c_addr);
  Wire.write(0x10);      // Set resolution to 1 Lux
  Wire.endTransmission();
  delay(300);
}
 
// void loop() 
void MeasureLightIntensity()
{
  if (millis() > (timebh1750 + TimeBetweenReadingBH1750)) {//retriving value of Lux, FtCd and Wattsm2 from BH1750
    timebh1750 = millis();
    unsigned int i=0;
    static float persistedll;
    static float persistedlf;
    static float persistedlw;
    unsigned int Lux;
    float FtCd;
    float Wattsm2;

    // Check if reads failed and exit early (to try again).
    Wire.requestFrom(BH1750_i2c_addr, 2);
    if(Wire.available() != 2) {
      trc(F("Failed to read from LightSensor BH1750!"));
    }else{
      i = Wire.read();
      i <<=8;
      i |= Wire.read(); 

      // Calculate the Values
      Lux = i/1.2;  // Convert to Lux
      FtCd = Lux/10.764;
      Wattsm2 = Lux/683.0;

      /*
      Useful Information ;-)
      lux (lx)                            # 1 lx = 1 lm/m² = 1 cd·sr·m⁻².
      meter-candle (m-cd)                 # 1 m·cd = 1 lx = 1 lm/m² = 1 cd·sr·m⁻².
      centimeter-candle (cm-sd)           # 1 m·cd = 1 lx = 1 lm/m² = 1 cd·sr·m⁻².
      foot-candle (ft-c)                  # 
      phot (ph)                           # 1 ph = 1 lm/cm² = 10,000 lm/m² - 10,000 lx = 10 klx
      nox (nx)                            # 1 nox = 1 millilux
      candela steradin/meter2(cd·sr·m⁻²)  # 1 lx = 1 lm/m² = 1 cd·sr·m⁻²
      lumen/meter2 (lm·m⁻²)               # 1 lx = 1 lm/m² = 1 cd·sr·m⁻²
      lumen/centimeter2 (lm·cm⁻²)         # 1 lm/cm² = 10,000 lx = 10,000 cd·sr·m⁻²
      lumen/foot2 (lm·ft⁻²)               # (lm·ft⁻²)
      watt/centimeter2 at 555nm  (W·cm⁻²) # 
      */

      // Generate Lux
      if(Lux != persistedll || bh1750_always){
        char lux[7];
        sprintf(lux,"%u",Lux);
        trc(F("Sending Lux to MQTT"));
        trc(String(lux));
        client.publish(LUX,lux);
       }else{
        trc(F("Same lux don't send it"));
       }

      // Generate FtCd
      if(FtCd != persistedlf || bh1750_always){
        char ftcd[7];
        dtostrf(FtCd,4,2,ftcd);
        trc(F("Sending FtCd to MQTT"));
        trc(String(ftcd));
        client.publish(FTCD,ftcd);
      }else{
        trc(F("Same ftcd don't send it"));
      }

      // Generate Watts/m2
      if(Wattsm2 != persistedlw || bh1750_always){
        char wattsm2[7];
        dtostrf(Wattsm2,6,4,wattsm2);
        trc(F("Sending Wattsm2 to MQTT"));
        trc(String(wattsm2));
        client.publish(WATTSM2,wattsm2);
      }else{
        trc(F("Same wattsm2 don't send it"));
      }    

    }
    persistedll = Lux;
    persistedlf = FtCd;
    persistedlw = Wattsm2;
  }
}

#endif
