#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H
#include "globals.h"
struct Queue {
    signed char id[QUEUE_SIZE];
    unsigned long time[QUEUE_SIZE];
    signed char state[QUEUE_SIZE];
    unsigned int i = 0; // current index in the queue
};

void logTouchToSD();
bool create_log_file();
void add_to_queue(signed char sipper_id, unsigned long timestamp, signed char state);

#endif // SD_FUNCTIONS_H