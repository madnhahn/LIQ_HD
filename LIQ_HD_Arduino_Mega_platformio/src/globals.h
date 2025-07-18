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


#if ENABLE_DEBUG
	#define DEBUG_PRINTLN(x) Serial.println(x)
	#define DEBUG_PRINT(x) Serial.print(x)
#else
	#define DEBUG_PRINTLN(x)
	#define DEBUG_PRINT(x)
#endif

extern Adafruit_MPR121 caps[NUM_SENSORS];

extern int touch_threshold;
extern int release_threshold;

#endif // GLOBALS_H	