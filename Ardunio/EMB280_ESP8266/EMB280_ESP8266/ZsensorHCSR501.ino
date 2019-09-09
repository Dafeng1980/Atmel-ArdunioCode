/*  
  OpenMQTTGateway Addon  - ESP8266 or Arduino program for home automation 

   Act as a wifi or ethernet gateway between your 433mhz/infrared IR signal  and a MQTT broker 
   Send and receiving command by MQTT
 
    HC SR-501 reading Addon
  
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
#ifdef ZsensorHCSR501

void setupHCSR501() {
  pinMode(HCSR501_PIN, INPUT);     // declare HC SR-501 pin as input
}

void MeasureHCSR501(){
  if (millis() > TimeBeforeStartHCSR501) {//let time to init the PIR sensor
  static int pirState = LOW;
  int PresenceValue = digitalRead(HCSR501_PIN);
  #ifdef ESP8266
    yield();
  #endif
  if (PresenceValue == HIGH) { 
    if (pirState == LOW) {
     // turned on
     client.publish(subjectHCSR501toMQTT,"true");
      trc(F("HC SR501 Motion detected"));
      pirState = HIGH;
    }
    } else {
      if (pirState == HIGH){
        // turned off
        client.publish(subjectHCSR501toMQTT,"false");
        trc(F("HC SR501 Motion ended"));
        pirState = LOW;
      }
    }
  }
}
#endif
