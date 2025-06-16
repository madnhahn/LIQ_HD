#include "sensor_functions.h"
#include "globals.h"
#include <Arduino.h>
#include <SdFat.h>

void logTouchToSD(int sipper_id, unsigned long timestamp, int state){
  File dataFile = SD.open(logFileName, FILE_WRITE);
  if (dataFile)
  {
    dataFile.print(sipper_id);
    dataFile.print(" , ");
    dataFile.print(timestamp);
    dataFile.print(" , ");
    dataFile.println(state);
    dataFile.close();

    Serial.print(sipper_id);
    Serial.print(" , ");
    Serial.print(timestamp);
    Serial.print(" , ");
    Serial.println(state);
  }
  else
  {
    Serial.println("Error opening log file");
  }
}


void write_SD_headers(){
  File dataFile = SD.open(logFileName, FILE_WRITE);
  if (dataFile)
  {
    dataFile.println("sipper_id , timestamp, state");
    dataFile.close();
  }
  else
  {
    Serial.println("Error opening logFileName");
  }
}