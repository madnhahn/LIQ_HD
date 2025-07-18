#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H

#include "globals.h"

// application notes for MPR121
// https://www.nxp.com/docs/en/application-note/AN3892.pdf

struct Settings {
	int touch_threshold = touch_threshold_default;
	int release_threshold = release_threshold_default;
};
void record(Settings settings);
void print_thresholds();
bool initialize_sensors(Settings settings);
void check_single_sensor(int sensor, unsigned long now, bool currently_licking[PADS_PER_SENSOR]);

#endif // SENSOR_FUNCTIONS_H