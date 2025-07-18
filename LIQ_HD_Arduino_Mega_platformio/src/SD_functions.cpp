#include "sensor_functions.h"
#include "globals.h"
#include <Arduino.h>
#include <SdFat.h>
#include "SD_functions.h"
#include <RTClib.h>


SdFat SD;
int id_list[QUEUE_SIZE];
unsigned long time_list[QUEUE_SIZE];
int state_list[QUEUE_SIZE];
int current_index = 0;
char logFileName[40];


void logTouchToSD(){
	DEBUG_PRINT("Logging to: "); DEBUG_PRINTLN(logFileName);
	File dataFile = SD.open(logFileName, FILE_WRITE);
	if (dataFile){
		for (int i = 0; i < current_index; i++) {
			dataFile.print(id_list[i]);
			dataFile.print(" , ");
			dataFile.print(time_list[i]);
			dataFile.print(" , ");
			dataFile.println(state_list[i]);
			
			DEBUG_PRINT(id_list[i]);
			DEBUG_PRINT(" , ");
			DEBUG_PRINT(time_list[i]);
			DEBUG_PRINT(" , ");
			DEBUG_PRINTLN(state_list[i]);
		}
		dataFile.close();
		current_index = 0; // reset the index to 0 if we reach the end of the list
	}
	else {
		DEBUG_PRINTLN("Error opening log file");
	}
}


void add_to_queue(int sipper_id, unsigned long timestamp, int state){
	id_list[current_index] = sipper_id;
	time_list[current_index] = timestamp;
	state_list[current_index] = state;
	current_index = current_index + 1;

	if (current_index >= QUEUE_SIZE){
		logTouchToSD();
	}
}

bool create_log_file(){
	if (!SD.begin(chipSelect)) {
		Serial.println("SD card initialization failed!");
		return false;
	}

	RTC_PCF8523 rtc;
	if (!rtc.begin()) {
		Serial.println("Couldn't find RTC");
		return false;
	}

	DateTime now = rtc.now();
	snprintf(logFileName, sizeof(logFileName),
		"licks_%04d%02d%02d_%02d%02d%02d.csv",
		now.year(), now.month(), now.day(),
		now.hour(), now.minute(), now.second());

	DEBUG_PRINT("Writing headers to:"); DEBUG_PRINTLN(logFileName);
	File dataFile = SD.open(logFileName, FILE_WRITE);
	if (dataFile) {
		dataFile.println("sipper_id , timestamp, state");
		dataFile.close();
		return true;
	}
	else {
		DEBUG_PRINT("Error creating "); DEBUG_PRINTLN(logFileName);
		return false;
	}
}

