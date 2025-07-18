#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H

#include "globals.h"

// application notes for MPR121
// https://www.nxp.com/docs/en/application-note/AN3892.pdf

void record();
void set_sensor_settings();
void print_thresholds();
void check_single_sensor(int sensor, unsigned long now, bool currently_licking[PADS_PER_SENSOR]);
bool lick_started(bool is_touched, int sensor, int pad, unsigned long now);
bool lick_stopped(bool is_touched, int sensor, int pad, unsigned long now);

#endif // SENSOR_FUNCTIONS_H