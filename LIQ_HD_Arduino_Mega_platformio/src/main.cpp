
#include <Arduino.h>
#include "globals.h"
#include "sensor_functions.h"
#include "SD_functions.h"
#include "serial_functions.h"

Settings settings;

void setup(){
	Serial.begin(115200);
	Serial.print("Initial free memory: "); Serial.println(freeMemory());
	pinMode(button1Pin, INPUT_PULLUP);
	pinMode(LED_PIN, OUTPUT);
	Serial.println("System ready.");
}

void loop() {
	if (digitalRead(button1Pin) == LOW) {
		Serial.println("Button 1 pressed. Starting recording...");
		digitalWrite(LED_PIN, HIGH); //turn LED on
		delay(400);
		record(settings);
		digitalWrite(LED_PIN, LOW); //turn LED off
	}
	// Check for serial commands to update settings
	String cmd = serial_command();
	if (cmd.length() > 0){
		update_settings(settings, cmd);
	}
}