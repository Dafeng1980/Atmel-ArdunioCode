/*
 Name:		ESP8266_I2C_sensor.ino
 Created:	12/17/2017 2:22:11 PM
 Author:	Dafeng
*/
#include <ESP8266WiFi.h>
#include "Wire.h"
#include "SparkFunBME280.h"
#include <PubSubClient.h>

#define TRACE 1

#define bme280_always true 
#define TimeBetweenReadingbme280 100000
unsigned long timebme280 = 0;

const char* ssid = "PeppaPig";
const char* password = "20182018";
const char* mqtt_server = "35.236.168.84";
const char* clientID = "device1";
const char* mqtt_user = "dfiot";
const char* mqtt_password = "123abc";
const uint16_t mqtt_port =  1883;

//const int SDA_PIN = 0;
//const int SDC_PIN = 2;

WiFiClient eClient;
PubSubClient client(mqtt_server, mqtt_port, callback, eClient);

//Global sensor object
BME280 mySensor;

void setupZsensorBME280()
{
	mySensor.settings.commInterface = I2C_MODE;
	mySensor.settings.I2CAddress = 0x76; // Bosch BME280 I2C Address  

										 //***Operation settings*****************************//

										 // runMode Setting - Values:
										 // -------------------------
										 //  0, Sleep mode
										 //  1 or 2, Forced mode
										 //  3, Normal mode
	mySensor.settings.runMode = 3; //Normal mode

								   // tStandby Setting - Values:
								   // --------------------------
								   //  0, 0.5ms
								   //  1, 62.5ms
								   //  2, 125ms
								   //  3, 250ms
								   //  4, 500ms
								   //  5, 1000ms
								   //  6, 10ms
								   //  7, 20ms
	mySensor.settings.tStandby = 0;

	// Filter can be off or number of FIR coefficients - Values:
	// ---------------------------------------------------------
	//  0, filter off
	//  1, coefficients = 2
	//  2, coefficients = 4
	//  3, coefficients = 8
	//  4, coefficients = 16
	mySensor.settings.filter = 0;

	// tempOverSample - Values:
	// ------------------------
	//  0, skipped
	//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
	mySensor.settings.tempOverSample = 1;

	// pressOverSample - Values:
	// -------------------------
	//  0, skipped
	//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
	mySensor.settings.pressOverSample = 1;

	// humidOverSample - Values:
	// -------------------------
	//  0, skipped
	//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
	mySensor.settings.humidOverSample = 1;

	delay(10); // Gives the Sensor enough time to turn on (The BME280 requires 2ms to start up)
	Serial.print("Bosch BME280 Initialized - Result of .begin(): 0x");
	Serial.println(mySensor.begin(), HEX);
}
// the setup function runs once when you press reset or power the board
void setup() {

	//I2C stuff
	Wire.pins(0, 2);
	Wire.begin();
	//Wire.begin(SDA_PIN, SDC_PIN);

	delay(30);
	setupZsensorBME280();
	delay(10);

	Serial.begin(115200);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	client.connect(clientID, mqtt_user, mqtt_password);
}


// the loop function runs over and over again until power down or reset
void loop() {
	
	if (!client.connected()) {
		reconnect();
	}
	client.loop();

	if (millis() > (timebme280 + TimeBetweenReadingbme280)) {
		timebme280 = millis();
		static float persisted_bme_tempc;
		//static float persisted_bme_tempf;
		static float persisted_bme_hum;
		static float persisted_bme_pa;
		//static float persisted_bme_altim;
		//static float persisted_bme_altift;

		float BmeTempC = mySensor.readTempC();
		//float BmeTempF = mySensor.readTempF();
		float BmeHum = mySensor.readFloatHumidity();
		float BmePa = mySensor.readFloatPressure()/100;
		//float BmeAltiM = mySensor.readFloatAltitudeMeters();
		//float BmeAltiFt = mySensor.readFloatAltitudeFeet();

		// Check if reads failed and exit early (to try again).
		if (isnan(BmeTempC) || isnan(BmeHum) || isnan(BmePa)) {
			trc(F("Failed to read from Weather Sensor BME280!"));
		}
		else {
			// Generate Temperature in degrees C
			if (BmeTempC != persisted_bme_tempc || bme280_always) {
				char bmetempc[7];
				dtostrf(BmeTempC, 4, 1, bmetempc);
				trc(F("Sending Degrees C to MQTT"));
				trc(String(bmetempc));
				client.publish("tempc", bmetempc);
			}
			else {
				trc(F("Same Degrees C don't send it"));
			}

			// Generate Humidity in percent
			if (BmeHum != persisted_bme_hum || bme280_always) {
				char bmehum[7];
				dtostrf(BmeHum, 4, 1, bmehum);
				trc(F("Sending Humidity to MQTT"));
				trc(String(bmehum));
				client.publish("hum", bmehum);
			}
			else {
				trc(F("Same Humidity don't send it"));
			}

			// Generate Pressure in Pa
			if (BmePa != persisted_bme_pa || bme280_always) {
				char bmepa[7];
				dtostrf(BmePa, 4, 2, bmepa);
				trc(F("Sending Pressure to MQTT"));
				trc(String(bmepa));
				client.publish("pres", bmepa);
			}
			else {
				trc(F("Same Pressure don't send it"));
			}



		}
		persisted_bme_tempc = BmeTempC;
		//persisted_bme_tempf = BmeTempF;
		persisted_bme_hum = BmeHum;
		persisted_bme_pa = BmePa;
		//persisted_bme_altim = BmeAltiM;
		//persisted_bme_altift = BmeAltiFt;

	}
  
}


void trc(String msg) {
	if (TRACE) {
		Serial.println(msg);
	}
}

void callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println();

	// Switch on the LED if an 1 was received as first character
	if ((char)payload[0] == '1') {
		digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
										  // but actually the LED is on; this is because
										  // it is acive low on the ESP-01)
	}
	else {
		digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
	}

}

void reconnect() {
	// Loop until we're reconnected
	client.connect(clientID, mqtt_user, mqtt_password);
	while (!client.connected()) {
		trc(F("MQTT connection...")); //F function enable to decrease sram usage
		// Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		//if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
			if (client.connect("ESP8266Client")) {
			//Serial.println("connected");
			trc(F("Connected to broker"));
			// Once connected, publish an announcement...
			client.publish("outTopic", "hello world");
			// ... and resubscribe
			client.subscribe("inTopic");
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}