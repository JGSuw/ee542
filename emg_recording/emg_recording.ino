
/*
 * Author: Joseph Sullivan
 * Course: EE542
 * 
 * This is a test of the emg measurement subsystem. The device
 * is sampled at 1 kilohertz, and every 5 seconds the data is printed
 * over the serial console, to be written in a CSV file.
 */

#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif
#include "emg_measurement.h"


#define TEST_PERIOD 8

#define TEST_SLEEP_MS 1

#define TEST_BUFFER_SIZE 3000

adc_meas_t buff[TEST_BUFFER_SIZE];
int buff_head = 0;

void setup() {
  Serial.begin(115200);
  SPI1.begin(6);
}

void loop() {
  
  emg_measurement_task();
  
  delay(TEST_SLEEP_MS);
  
  if (adcQueueFull()) {
    int count; 
    adc_meas_t * buffptr = buff + buff_head;
    count = adcQueueReceive(buffptr);
    buff_head += count;

    if (buff_head == TEST_BUFFER_SIZE) {
      buff_head = 0;
      for(int i = 0; i < TEST_BUFFER_SIZE; i++) {
        Serial.print(buff[i].channel_A);
        if( i != TEST_BUFFER_SIZE-1) {
          Serial.print(',');
        } else { Serial.print('\n');}
      } 
    }
  }
}
