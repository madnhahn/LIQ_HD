#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H
#include "globals.h"

bool create_log_file();
void log_to_SD(signed char sipper_id, unsigned long timestamp, signed char state);
void close_log_file();


#endif // SD_FUNCTIONS_H