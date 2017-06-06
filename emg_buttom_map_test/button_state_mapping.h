#ifndef BUTTON_MAPING_HEADER
#define BUTTON_MAPING_HEADER
#include <stdbool.h>

bool getLMB();

bool getRMB();

void button_state_mapping_task();

void debug_buffer(adc_meas_t *);

#endif
