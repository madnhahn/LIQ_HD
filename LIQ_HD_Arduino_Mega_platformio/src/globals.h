#ifndef GLOBALS_H
#define GLOBALS_H

/////////////// Settings you might want to change //////////////////
#define NUM_SENSORS 3
#define PADS_PER_SENSOR 12
#define touch_threshold_default 5   // default = 12
#define release_threshold_default 3 // default = 6
#define button1Pin 48
#define LED_PIN 38
#define chipSelect 7
#define ENABLE_DEBUG false
#define LOG_LOOP_TIME true
///////////////////////////////////////////////////////////
#include <Adafruit_MPR121.h>
#include <SdFat.h>
#include <RTClib.h>

#if ENABLE_DEBUG
	#define DEBUG_PRINTLN(x) Serial.println(x)
	#define DEBUG_PRINT(x) Serial.print(x)
#else
	#define DEBUG_PRINTLN(x)
	#define DEBUG_PRINT(x)
#endif

extern SdFat SD;
extern char logFileName[32]; // name of the log file on the SD card
extern RTC_PCF8523 rtc;
extern Adafruit_MPR121 caps[NUM_SENSORS];
extern bool currently_licking[NUM_SENSORS][PADS_PER_SENSOR];
extern unsigned long experiment_start_time;

extern int touch_threshold;
extern int release_threshold;

#endif // GLOBALS_H	