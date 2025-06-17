#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H

void logTouchToSD();
void write_SD_headers();
void add_to_queue(int sipper_id, unsigned long timestamp, int state);

#endif // SD_FUNCTIONS_H