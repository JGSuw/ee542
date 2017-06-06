// Author: Joseph Sullivan
// Course: EE542

/*
 * This is a test of button state mapping. It runs the emg measurement task, and button state mapping task,
 * and prints to the serial console when a button is clicked.
 */


#if defined(ARDUINO)
SYSTEM_MODE(SEMI_AUTOMATIC);
#endif

#define SAMPLE_PERIOD_US 1000
#define UPDATE_PERIOD_US 8000

#include "hw_ticks.h"
#include "emg_measurement.h"
#include "button_state_mapping.h"

adc_meas_t measurement_buffer[50];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI1.begin(6);
}

void loop() {
  // put your main code here, to run repeatedly:
  static system_tick_t tick = GetSystem1UsTick();
  static system_tick_t tock = tick;
  
  system_tick_t now = GetSystem1UsTick();
  
  if ( now - tick > SAMPLE_PERIOD_US) {
    emg_measurement_task();
    //Serial.print("BAR");
    if (now - tock > UPDATE_PERIOD_US) {
      
      button_state_mapping_task();
      if(getRMB()) {
        Serial.println("CLICK!");
      } else {
        Serial.println();
      }
      tock = now;
    }
    tick = now;
  }
  
}
