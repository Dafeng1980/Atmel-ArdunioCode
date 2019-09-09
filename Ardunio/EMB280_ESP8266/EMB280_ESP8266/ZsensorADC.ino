/*  
  OpenMQTTGateway Addon  - ESP8266 or Arduino program for home automation 

   Act as a wifi or ethernet gateway between your 433mhz/infrared IR signal  and a MQTT broker 
   Send and receiving command by MQTT
 
    Analog pin reading Addon
  
    Copyright: (c)Florian ROBERT
    
    Contributors:
    - 1technophile
  
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
#ifdef ZsensorADC

#ifdef ESP8266
  ADC_MODE(ADC_TOUT);
#endif

//Time used to wait for an interval before resending adc value
unsigned long timeadc = 0;

void MeasureADC(){
  if (millis() > (timeadc + TimeBetweenReadingADC)) {//retriving value of temperature and humidity of the box from DHT every xUL
    timeadc = millis();
    static int persistedadc;
    int val = analogRead(ADC_PIN);
    #ifdef ESP8266
      yield();
    #endif
    if (isnan(val)) {
      trc(F("Failed to read from ADC !"));
    }else{
      if(val  >= persistedadc + ThresholdReadingADC || val  <= persistedadc - ThresholdReadingADC){
        char value[4];
        sprintf(value, "%d", val);
        trc(F("Sending analog value to MQTT"));
        trc(String(val));
        client.publish(ADC,value);
        persistedadc = val;
       }
    }
  }
}
#endif

