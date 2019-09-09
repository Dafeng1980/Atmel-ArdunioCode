#include <EEPROM.h>
#define TURN_OFF 0x00 
#define PORTNUMBER_B 1     //portB
#define  ON_ALL  0
#define  STANDBY  1
#define  NORMAL  2
#define  LIGHT1  3
#define  LIGHT2  4

const byte Address=0;   //internal EEPROM Addr
byte switchstatus[]{0x07, 0x01, 0x06, 0x02, 0x04};
byte switchcycle; 

/*** enum cycles{
  ON_ALL,
  STANDBY,
  NORMAL,
  LIGHT1,
  LIGHT2,
}cycle;  ***/

void setup() {
  portMode(PORTNUMBER_B, OUTPUT);
  portWrite(PORTNUMBER_B, TURN_OFF);
  
  EEPROM.get(Address,switchcycle);
  portWrite(PORTNUMBER_B, switchstatus[switchcycle]);
  switchcycle++;
 if(switchcycle >4){
 switchcycle = 1;
 }
 EEPROM.put(Address, switchcycle);  
 
 delay(1000);
  
  // put your setup code here, to run once:

}

void loop() {
  delay(3000);
  portWrite(PORTNUMBER_B,switchstatus[ON_ALL]);
  delay(2000);
  portWrite(PORTNUMBER_B,switchstatus[NORMAL]);
  // put your main code here, to run repeatedly:

}
