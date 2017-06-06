
#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif
#include "emg_measurement.h"


#define TEST_PERIOD 8

#define TEST_SLEEP_MS 1

adc_meas_t buff[ADC_BUFFER_SIZE];

void setup() {
  Serial.begin(115200);
  SPI1.begin(6);
}

void loop() {
  emg_measurement_task();
  delay(TEST_SLEEP_MS);
  if (adcQueueFull()) {
    adcQueueReceive(buff);
    for(int i = 0; i < ADC_BUFFER_SIZE; i++) {
      Serial.print(buff[i].channel_A);
      if( i != ADC_BUFFER_SIZE-1) {
        Serial.print(',');
      } else { Serial.print('\n');}
    }
  }
}
