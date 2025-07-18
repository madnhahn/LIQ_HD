#include <Arduino.h>
#include "globals.h"
#include "sensor_functions.h"



String serial_command() {
	// updates string buffer
	// returns true as soon as a newline character is encountered
	static String inputBuffer = "";
	while (Serial.available()) {
		char c = Serial.read();
		Serial.print(c);
		if (c == '\n') {
			String result = inputBuffer;
			inputBuffer = ""; // Clear buffer for next command
			return result;
		}
		else {
			inputBuffer += c;
		}
	}
	return ""; // Return empty string if no complete command is available
}

void update_settings(Settings settings, String cmd) {
	cmd.trim();
	if (cmd.length() < 2) {
		Serial.println("Invalid entry: input too short");
		return;
	}

	int value = cmd.substring(1).toInt();
	if (cmd[0] == 't'){
		Serial.print("Set touch threshold to: "); Serial.println(value);
		settings.touch_threshold = value;
		return;
	}
	else if(cmd[0] == 'r'){
		Serial.print("Set release threshold to: "); Serial.println(value);
		settings.release_threshold = value;
		return;
	}
	else {
		Serial.println("Invalid entry: Must start with 't' or 'r' to set touch or release threshold");
		return;
	}

}




#ifdef __arm__ // should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}