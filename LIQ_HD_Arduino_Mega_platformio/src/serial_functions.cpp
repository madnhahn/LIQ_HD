#include <Arduino.h>
#include "globals.h"
#include "sensor_functions.h"

void setThresholdsFromSerialInput() {
	static String inputBuffer = "";
	while (Serial.available()) {
		char c = Serial.read();
		if (c == '\n') {
			inputBuffer.trim();
			if (inputBuffer.length() > 0) {
				int value = inputBuffer.substring(1).toInt();
				if (inputBuffer[0] == 't'){
					Serial.print("Set touch threshold to: ");
					Serial.println(value);
					touch_threshold = value;
				}
				else if(inputBuffer[0] == 'r'){
					Serial.print("Set release threshold to: ");
					Serial.println(value);
					release_threshold = value;
				}
				set_sensor_settings();
			}
			inputBuffer = ""; // Clear buffer for next command
		}
		else {
			inputBuffer += c;
		}
	}
}