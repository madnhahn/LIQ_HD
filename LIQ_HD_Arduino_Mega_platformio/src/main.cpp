
#include <Arduino.h>
#include "globals.h"
#include "sensor_functions.h"
#include "SD_functions.h"
#include "serial_functions.h"


Adafruit_MPR121 caps[NUM_SENSORS];

int touch_threshold = touch_threshold_default;
int release_threshold = release_threshold_default;

void setup(){
	Serial.begin(115200);
	Serial.print("Initial free memory: "); Serial.println(freeMemory());
	pinMode(button1Pin, INPUT_PULLUP);
	pinMode(LED_PIN, OUTPUT);

	const uint8_t sensorAddresses[NUM_SENSORS] = {0x5A, 0x5B, 0x5C};
	for (int i = 0; i < NUM_SENSORS; i++) {
		if (!caps[i].begin(sensorAddresses[i])) {
			Serial.print("MPR121 #");
			Serial.print(i + 1);
			Serial.print(" not found at 0x");
			Serial.println(sensorAddresses[i], HEX);
			while (1){;}
		}
	}

	set_sensor_settings();

	Serial.println("System ready.");
}

void loop() {
	if (digitalRead(button1Pin) == LOW) {
		Serial.println("Button 1 pressed. Starting recording...");
		digitalWrite(LED_PIN, HIGH); //turn LED on
		delay(400);
		record();
		digitalWrite(LED_PIN, LOW); //turn LED off
		Serial.println("Button 1 pressed. Stopping recording...");
		delay(300);
	}
	setThresholdsFromSerialInput();
}