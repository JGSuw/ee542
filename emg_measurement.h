#ifndef EMG_HEADER
#define EMG_HEADER
#include <stdbool.h>

typedef struct adc_meas_t{
  float channel_A;
  uint16_t channel_A_raw;
  float channel_B;
  uint16_t channel_B_raw;
};

typedef struct adc_meas_queue_t{
  const uint8_t queue_size;
  adc_meas_t buffer[queue_size];
};

uint8_t queueReceive(adc_meas_t[]);

bool queueFull(void);

#endif
