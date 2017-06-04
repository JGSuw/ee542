
#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif
#include "emg_measurement.h"


#define TEST_PERIOD 8

#define TEST_SLEEP_MS 10

adc_meas_t buff[ADC_BUFFER_SIZE];

void setup() {
  Serial.begin(57600);
}

void loop() {

  emg_measurement_task();

  if (adcQueueFull()) {
    adcQueueReceive(buff);
    for(int i = 0; i < ADC_BUFFER_SIZE; i++) {
      Serial.print(buff[i].channel_A);
      Serial.print(" ");
      Serial.print(buff[i].channel_B);
      Serial.println();
    }
  }
}
