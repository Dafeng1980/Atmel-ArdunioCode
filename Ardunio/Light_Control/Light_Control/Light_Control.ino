
#include "fwb.h"
#include <arduino.h>
#include <EEPROM.h>

#define INPUT_KEY 10
#define TURN_OFF 0x00 
#define PORTNUMBER_B 1     //portB
#define  ON_ALL  0
#define  STANDBY  1
#define  NORMAL  2
#define  LIGHT1  3
#define  LIGHT2  4
#define BP 11 //the pin where your button is connected
//
 FWB bp;
 
  void OnClick(int pin) {
	  
	  lights(STANDBY);
	  Serial.println("OnClick");
    //Your code here
  }
 
  void OnDblClick(int pin) {
	  
	  lights(2);
	  Serial.println("OnDbClick");
    //Your code here
  }
 
  void OnLongPress(int pin) {
    //Your code here
  }
 
  void OnVLongPress(int pin) {
    //Your code here
  }
const char Address=0;   //internal EEPROM Addr

const uint8_t lightstatus[]{0x0E, 0x02, 0x0C, 0x04, 0x08};
	
uint8_t switchcycle;
 
//enum Lightmodes{
	//ON_ALL,
	//STANDBY,
	//NORMAL,
	//LIGHT1,
	//LIGHT2
	//};


 void lights(int i)
{
	portWrite(PORTNUMBER_B, lightstatus[i]);
}

void setup() {
	Serial.begin(38400);
	bp.Configure(BP);
	 bp.OnClick = OnClick;
	 bp.OnDblClick = OnDblClick;
	 bp.OnLongPress = OnLongPress;
	 bp.OnVLongPress = OnVLongPress;
	   
 // pinMode(INPUT_KEY, INPUT_PULLUP);
	
  portMode(PORTNUMBER_B, OUTPUT);
  
  portWrite(PORTNUMBER_B, TURN_OFF);
  
  EEPROM.get(Address,switchcycle);
  
  portWrite(PORTNUMBER_B, lightstatus[switchcycle]);
  
  switchcycle++;
  
 if(switchcycle >4){
	 
 switchcycle = 1;
 
 }
 
 EEPROM.put(Address, switchcycle);
   
 delay(1000);
 
}

void loop() {
	
	bp.CheckBP();
  //delay(3000);
  //lights(ON_ALL);
  //delay(2000);
  //lights(NORMAL);
  //delay(2000);
  //lights(LIGHT1);
}