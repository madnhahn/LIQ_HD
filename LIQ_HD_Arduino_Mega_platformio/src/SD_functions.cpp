#include "sensor_functions.h"
#include "globals.h"
#include <Arduino.h>
#include <SdFat.h>
#include "SD_functions.h"
#include <RTClib.h>


SdFat SD;
Queue queue;
char logFileName[40];


void logTouchToSD(){
	DEBUG_PRINT("Logging to: "); DEBUG_PRINTLN(logFileName);
	File dataFile = SD.open(logFileName, FILE_WRITE);
	for (unsigned int i = 0; i < queue.i; i++) {
		dataFile.print(queue.id[queue.i]);
		dataFile.print(" , ");
		dataFile.print(queue.time[queue.i]);
		dataFile.print(" , ");
		dataFile.println(queue.state[queue.i]);
	}
	dataFile.close();
	queue.i = 0; // reset the index to 0 after writing to the file
}

void add_to_queue(signed char sipper_id, unsigned long timestamp, signed char state){
	queue.id[queue.i] = sipper_id;
	queue.time[queue.i] = timestamp;
	queue.state[queue.i] = state;
	queue.i += 1;

	if (queue.i >= QUEUE_SIZE){
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

