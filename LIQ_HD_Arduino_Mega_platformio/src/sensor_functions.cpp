#include "SD_functions.h"
#include "sensor_functions.h"
#include "globals.h"

void record(){
	bool currently_licking[NUM_SENSORS][PADS_PER_SENSOR] = {false};

	if (!create_log_file()) { return;}

	unsigned long experiment_start_time = millis(); // set the experiment start time to the current time
	unsigned long now;
	while (true) {
		for (int sensor = 0; sensor < NUM_SENSORS; sensor++){
			now = millis() - experiment_start_time;
			check_single_sensor(sensor, now, currently_licking[sensor]);
		}
		if (LOG_LOOP_TIME) {
			now = millis() - experiment_start_time;
			add_to_queue(-1, now, 1);
		}
		if(digitalRead(button1Pin) == LOW){
			logTouchToSD();
			break;
		}
	}
}

void check_single_sensor(int sensor, unsigned long now, bool currently_licking[PADS_PER_SENSOR]) {
	uint16_t touched = caps[sensor].touched();            // bit array with 1s and 0s meaning touched vs. not touched for each of 12 sippers
	for (uint8_t pad = 0; pad < PADS_PER_SENSOR; pad++) { // loop through each sipper by bitmasking againt shifted 00001
		bool is_touched = touched & (1 << pad);
		int sipper_id = sensor * PADS_PER_SENSOR + pad; // Compute the sipper ID counting from 0 to 35 accross the 3 capacitive touch sensors

		// check if new lick has started
		if (is_touched && !currently_licking[pad]) { // If this sipper registers as being touched and is not already mid-lick
			currently_licking[pad] = true;      // We are now considered currently licking
			DEBUG_PRINT("lick start detected on sipper "); DEBUG_PRINT(sipper_id); DEBUG_PRINT(" at "); DEBUG_PRINTLN(now);
			add_to_queue(sipper_id, now, 1); // Log the sipper time with state=1, meaning this is lick start
		}
		// check if current lick has stopped
		else if (!is_touched && currently_licking[pad]){ // If was previousluy licking, but is now not touched
			currently_licking[pad] = false;     // We are now considered not currently licking
			DEBUG_PRINT("lick stop detected on sipper ");DEBUG_PRINT(sipper_id); DEBUG_PRINT(" at "); DEBUG_PRINTLN(now);
			add_to_queue(sipper_id, now, 0);            // Log the sipper time with state=0, meaning this is lick stop
		}
	}
}

void set_sensor_settings(){
	// int debounce = 0; // 0 = turned off. Complicated bit array setting to set the actual range of touched and released (see application notes)      //range 0-7
	for (int sensor = 0; sensor < NUM_SENSORS; sensor++){
		caps[sensor].setThresholds(touch_threshold, release_threshold); // set sensitivity of touch and release of capacitive sensors
		// cap.writeRegister(MPR121_CONFIG1, current);             // default, 16uA charge current

		// See all register meaning and defaults at https://www.nxp.com/docs/en/data-sheet/MPR121.pdf
		// caps[sensor].writeRegister(MPR121_DEBOUNCE, debounce);
		// caps[sensor].writeRegister(MPR121_MHDR, 1);  // Max Half Delta Rising
		// caps[sensor].writeRegister(MPR121_NHDR, 10); // Noise Half Delta Rising
		// caps[sensor].writeRegister(MPR121_NCLR, 1); // Noise Count Limit Rising
		// caps[sensor].writeRegister(MPR121_FDLR, 2); // Falling Delta Rising

		// caps[sensor].writeRegister(MPR121_MHDF, 1);  // Max Half Delta Falling
		// caps[sensor].writeRegister(MPR121_NHDF, 5); // Noise Half Delta Falling
		// caps[sensor].writeRegister(MPR121_NCLF, 3); // Noise Count Limit Falling
		// caps[sensor].writeRegister(MPR121_FDLF, 1); // Falling Delta Falling

		// caps[sensor].writeRegister(MPR121_NHDT, 1); // Noise Half Delta Touch
		// caps[sensor].writeRegister(MPR121_NCLT, 5); // Noise Count Limit Touch
		// caps[sensor].writeRegister(MPR121_FDLT, 1); // Falling Delta Touch
	}
}

void print_thresholds() {
	for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {
		Serial.print("Sensor ");
		Serial.print(sensor);
		Serial.println(":");
		for (int pad = 0; pad < PADS_PER_SENSOR; pad++) {
			uint8_t reg = 0x41 + pad * 2; // Touch threshold register for electrode N
			uint8_t threshold = caps[sensor].readRegister8(reg);
			Serial.print("  Pad ");
			Serial.print(pad);
			Serial.print(" threshold: ");
			Serial.println(threshold);
		}
	}
}