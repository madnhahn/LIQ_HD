#include <Arduino.h>
#include "globals.h"
#include "sensor_functions.h"

static String inputBuffer = "";

bool update_buffer() {
	// updates string buffer
	// returns true as soon as a newline character is encountered
	while (Serial.available()) {
		char c = Serial.read();
		Serial.print(c);
		if (c == '\n') {
			return true;
		}
		else {
			inputBuffer += c;
		}
	}
	return false;
}

bool update_thresh_global(){
	inputBuffer.trim();
	if (inputBuffer.length() < 2) {
		Serial.println("Invalid entry: input too short");
		return false;
	}

	int value = inputBuffer.substring(1).toInt();
	if (inputBuffer[0] == 't'){
		Serial.print("Set touch threshold to: "); Serial.println(value);
		touch_threshold = value;
		return true;
	}
	else if(inputBuffer[0] == 'r'){
		Serial.print("Set release threshold to: "); Serial.println(value);
		release_threshold = value;
		return true;
	}
	else {
		Serial.println("Invalid entry: Must start with 't' or 'r' to set touch or release threshold");
		return false;
	}
}

void setThresholdsFromSerialInput() {
	bool line_complete = update_buffer();
	if (line_complete) {
		bool parsed = update_thresh_global();
		if (parsed) {set_sensor_settings();}
		inputBuffer = ""; // Clear buffer for next command
	}
}

