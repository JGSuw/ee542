#ifndef EMG_HEADER
#define EMG_HEADER
#include <stdbool.h>
#define ADC_BUFFER_SIZE 8

typedef struct{
  float channel_A;
  uint16_t channel_A_raw;
  float channel_B;
  uint16_t channel_B_raw;
}adc_meas_t;

uint8_t adcQueueReceive(adc_meas_t[]);

bool adcQueueFull(void);

void emg_measurement_task(void);

#endif

