#include "sensor_functions.h"
#include "globals.h"
#include <Arduino.h>
#include <SdFat.h>
#include "SD_functions.h"
#include <RTClib.h>

File dataFile;

bool create_log_file(){
	SdFat SD;
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
	char logFileName[40];
	snprintf(logFileName, sizeof(logFileName),
		"licks_%04d%02d%02d_%02d%02d%02d.csv",
		now.year(), now.month(), now.day(),
		now.hour(), now.minute(), now.second());

	dataFile = SD.open(logFileName, FILE_WRITE);
	if (!dataFile) {
		Serial.print("Error creating "); Serial.println(logFileName);
		return false;
	}

	Serial.print("Writing headers to:"); Serial.println(logFileName);
	dataFile.println("sipper_id,timestamp,state");
	return true;
}

void log_to_SD(signed char sipper_id, unsigned long timestamp, signed char state){
	static char buf[32];
	static unsigned long last_flush = millis();;

	snprintf(buf, sizeof(buf), "%d,%lu,%d\n", sipper_id, timestamp, state);
	dataFile.write(buf, strlen(buf));

	if (timestamp-last_flush >= MAX_BUFFER_TIME){
		dataFile.flush();
		last_flush = timestamp;
	}
}

void close_log_file(){
	if (dataFile) {
		dataFile.close();
		Serial.println("Log file closed.");
	} else {
		Serial.println("No log file to close.");
	}
}