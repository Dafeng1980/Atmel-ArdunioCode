
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RF24.h"


Adafruit_SSD1306 display;
/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(10, 11);
/**********************************************************/

byte addresses[][6] = { "1Node","2Node" };

// Used to control whether this node is sending or receiving
bool role = 1;

void setup() {
	Serial.begin(115200);
	Serial.println(F("RF24/examples/GettingStarted"));
	Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.clearDisplay();
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.setCursor(0, 0);
	display.println("Redead");
	display.display();
	delay(2000);
	
	radio.begin();

	// Set the PA Level low to prevent power supply related issues since this is a
	// getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_250KBPS);

	// Open a writing and reading pipe on each radio, with opposite addresses
	if (radioNumber) {
		radio.openWritingPipe(addresses[1]);
		radio.openReadingPipe(1, addresses[0]);
	}
	else {
		radio.openWritingPipe(addresses[0]);
		radio.openReadingPipe(1, addresses[1]);
	}

	// Start the radio listening for data
	radio.startListening();
}

void loop() {

	//role = 1;
	/****************** Ping Out Role ***************************/
	if (role == 1) {

		radio.stopListening();                                    // First, stop listening so we can talk.


		Serial.println(F("Now sending"));
		display.clearDisplay();
		// text display tests
		display.setTextSize(2);
		//display.setTextColor(WHITE);
		display.setCursor(0, 24);
		display.println("NW Sending");
		display.display();

		unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
		if (!radio.write(&start_time, sizeof(unsigned long))) {
			Serial.println(F("failed"));
			display.clearDisplay();
			// text display tests
			display.setTextSize(2);
			//display.setTextColor(WHITE);
			display.setCursor(0, 36);
			display.println("Failed");
			display.display();
		}

		radio.startListening();                                    // Now, continue listening

		unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
		boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not

		while (!radio.available()) {                             // While nothing is received
			if (micros() - started_waiting_at > 200000) {            // If waited longer than 200ms, indicate timeout and exit while loop
				timeout = true;
				break;
			}
		}

		if (timeout) {                                             // Describe the results
			Serial.println(F("Failed, response timed out."));
			display.clearDisplay();
			// text display tests
			display.setTextSize(1);
			display.setCursor(0, 0);
			display.println("Failed,responseTO");
			display.display();
		}
		else {
			unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
			radio.read(&got_time, sizeof(unsigned long));
			unsigned long end_time = micros();

			// Spew it
			Serial.print(F("Sent "));
			display.clearDisplay();
			// text display tests
			display.setTextSize(1);
			display.setCursor(0, 0);
			display.println("Sent");
			Serial.print(start_time);			
			display.println(start_time);
			display.println();
			Serial.print(F(", Got response "));
			display.println("Got response ");
			Serial.print(got_time);
			display.println(got_time);
			display.println();
			
			Serial.print(F(", Round-trip delay "));
			display.println("Round-trip delay");
			
			Serial.print(end_time - start_time);
			display.print(end_time - start_time);
			
			Serial.println(F(" microseconds"));
			display.println(" Microseconds");
			display.display();
		}

		// Try again 1s later
		delay(1000);
	}


	/****************** Pong Back Role ***************************/

	if (role == 0)
	{
		unsigned long got_time;

		if (radio.available()) {
			// Variable for the received timestamp
			while (radio.available()) {                                   // While there is data ready
				radio.read(&got_time, sizeof(unsigned long));             // Get the payload
			}

			radio.stopListening();                                        // First, stop listening so we can talk   
			radio.write(&got_time, sizeof(unsigned long));              // Send the final one back.      
			radio.startListening();                                       // Now, resume listening so we catch the next packets.     
			Serial.print(F("Sent response "));
			Serial.println(got_time);
		}
	}


	/****************** Change Roles via Serial Commands ***************************/

	if (Serial.available())
	{
		char c = toupper(Serial.read());
		if (c == 'T' && role == 0) {
			Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
			role = 1;                  // Become the primary transmitter (ping out)

		}
		else
			if (c == 'R' && role == 1) {
				Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
				role = 0;                // Become the primary receiver (pong back)
				radio.startListening();

			}
	}


} // Loop
