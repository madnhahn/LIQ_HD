#ifndef GLOBALS_H
#define GLOBALS_H

/////////////// Settings you might want to change //////////////////
#define NUM_SENSORS 3
#define PADS_PER_SENSOR 12
#define touch_threshold_default 7   // default = 12
#define release_threshold_default 3 // default = 6
#define off_debounce 40       // time in milliseconds to wait before declaring a lick has stopped (maybe smarter to use sensor's built in debounce? But complicated.)
#define on_debounce 40        // time in milliseconds to wait before declaring a lick has started
#define button1Pin 48
#define LED_PIN 38
#define chipSelect 7
#define ENABLE_DEBUG 0
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
extern unsigned long lick_start_time[NUM_SENSORS][PADS_PER_SENSOR];
extern unsigned long lick_stop_time[NUM_SENSORS][PADS_PER_SENSOR];
extern bool currently_licking[NUM_SENSORS][PADS_PER_SENSOR];
extern unsigned long experiment_start_time;

extern int touch_threshold;
extern int release_threshold;

#endif // GLOBALS_H	