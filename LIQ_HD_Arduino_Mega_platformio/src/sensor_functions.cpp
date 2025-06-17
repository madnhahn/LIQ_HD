#include "SD_functions.h"
#include "sensor_functions.h"
#include "globals.h"

void initialize_variables(){
  for (int sensor = 0; sensor < NUM_SENSORS; sensor++)
  {
    for (uint8_t pad = 0; pad < PADS_PER_SENSOR; pad++)
    {
      lick_start_time[sensor][pad] = 0;
      lick_stop_time[sensor][pad] = 0;
      currently_licking[sensor][pad] = false;
    }
  }
}

void record(){
  initialize_variables();
  DateTime now = rtc.now();
  snprintf(logFileName, sizeof(logFileName), "%04d%02d%02d_%02d%02d%02d.csv", now.year() % 100, now.month(), now.day(), now.hour(), now.minute(), now.second());
  experiment_start_time = millis(); // set the experiment start time to the current time
  write_SD_headers();               // Write the headers to the SD card file

//  unsigned long start_of_timer = millis(); //used for timing pulling rate
  while (true)
  {
    // Serial.println(millis() - start_of_timer); //used for timing pulling rate
    // start_of_timer = millis(); //used for timing pulling rate
    for (int sensor = 0; sensor < NUM_SENSORS; sensor++)
    {
      check_single_sensor(sensor);
    }
  }
}

void check_single_sensor(int sensor){
      uint16_t touched = caps[sensor].touched();            // bit array with 1s and 0s meaning touched vs. not touched for each of 12 sippers
      unsigned long now = millis() - experiment_start_time; // time of recording the touched status (time in milliseconds since the experiment started).
      for (uint8_t pad = 0; pad < PADS_PER_SENSOR; pad++)
      { // loop through each sipper by bitmasking againt shifted 00001
        bool is_touched = touched & (1 << pad);
        if (lick_started(is_touched, sensor, pad, now)) {
          currently_licking[sensor][pad] = true;      // We are now considered currently licking
          lick_start_time[sensor][pad] = now;         // We save the time this lick started
          int sipper_id = sensor * PADS_PER_SENSOR + pad; // Compute the sipper ID counting from 0 to 35 accross the 3 capacitive touch sensors
          add_to_queue(sipper_id, now, 1); // Log the sipper time with state=1, meaning this is lick start

        }
        else if (lick_stopped(is_touched, sensor, pad, now)){
          currently_licking[sensor][pad] = false;     // We are now considered not currently licking
          lick_stop_time[sensor][pad] = now;          // We save the time this lick stopped
          int sipper_id = sensor * PADS_PER_SENSOR + pad; // Compute the sipper ID counting from 0 to 35 accross the 3 capacitive touch sensors
          add_to_queue(sipper_id, now, 0);            // Log the sipper time with state=0, meaning this is lick stop
        }
      }

}

bool lick_started(bool is_touched, int sensor, int pad, unsigned long now){
  if (is_touched) { // If this sipper registers as being touched
    if (!currently_licking[sensor][pad]) { // If this sipper is not already mid-lick
      if (now - lick_stop_time[sensor][pad] > off_debounce){ // If sufficient time has passed since the last lick stopped
      return true;
      }
    }
  }
  return false;
}

bool lick_stopped(bool is_touched, int sensor, int pad, unsigned long now){
  if(!is_touched){ // If this sipper registers as NOT being touched
    if (currently_licking[sensor][pad]){ // If this sipper is mid-lick
      if (now - lick_start_time[sensor][pad] > on_debounce){ // If sufficient time has passed since the last lick started
      return true;
      }
    }
  }
  return false;
}


void set_sensor_settings(){
  int debounce = 0; // 0 = turned off. Complicated bit array setting to set the actual range of touched and released (see application notes)      //range 0-7
  for (int sensor = 0; sensor < NUM_SENSORS; sensor++)
  {
    caps[sensor].setThresholds(touch_threshold, release_threshold); // set sensitivity of touch and release of capacitive sensors
    // cap.writeRegister(MPR121_CONFIG1, current);             // default, 16uA charge current

    caps[sensor].writeRegister(MPR121_DEBOUNCE, debounce);
    caps[sensor].writeRegister(MPR121_MHDR, 1);
    caps[sensor].writeRegister(MPR121_NHDR, 10);
    caps[sensor].writeRegister(MPR121_NCLR, 1);
    caps[sensor].writeRegister(MPR121_FDLR, 2);

    caps[sensor].writeRegister(MPR121_MHDF, 1);
    caps[sensor].writeRegister(MPR121_NHDF, 5);
    caps[sensor].writeRegister(MPR121_NCLF, 3);
    caps[sensor].writeRegister(MPR121_FDLF, 1);

    caps[sensor].writeRegister(MPR121_NHDT, 1);
    caps[sensor].writeRegister(MPR121_NCLT, 5);
    caps[sensor].writeRegister(MPR121_FDLT, 1);
  }
}

void print_thresholds() {
    for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {
        Serial.print("Sensor ");
        Serial.print(sensor);
        Serial.println(":");
        for (int pad = 0; pad < PADS_PER_SENSOR; pad++) {
            uint8_t reg = 0x41 + pad * 2; // Touch threshold register for electrode N
            uint8_t threshold = caps[sensor].readRegister8(reg);
            Serial.print("  Pad ");
            Serial.print(pad);
            Serial.print(" threshold: ");
            Serial.println(threshold);
        }
    }
}