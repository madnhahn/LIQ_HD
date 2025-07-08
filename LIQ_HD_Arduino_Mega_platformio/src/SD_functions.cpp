#include "sensor_functions.h"
#include "globals.h"
#include <Arduino.h>
#include <SdFat.h>
#include "SD_functions.h"

const int list_length = 20;
int id_list[list_length];
unsigned long time_list[list_length];
int state_list[list_length];
int current_index = 0;

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

	if (current_index >= list_length){
		logTouchToSD();
	}
}

void write_SD_headers(){
	DEBUG_PRINT("Writing headers to:"); DEBUG_PRINTLN(logFileName);
	File dataFile = SD.open(logFileName, FILE_WRITE);
	if (dataFile) {
		dataFile.println("sipper_id , timestamp, state");
		dataFile.close();
	}
	else {
		DEBUG_PRINTLN("Error opening logFileName");
	}
}

