/* 
	Editor: http://www.visualmicro.com
			visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
			all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: ATmega644, Platform=avr, Package=MightyCore-master
*/

#define ARDUINO 10801
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU {build.f_cpu}
#define ARDUINO 10801
#define ARDUINO_AVR_ATmega644
#define ARDUINO_ARCH_AVR
void OnClick(int pin);
void OnDblClick(int pin);
void OnLongPress(int pin);
void OnVLongPress(int pin);
void lights(int i);
//
//

#include "Arduino.h"
#include "Light_Control.ino"
