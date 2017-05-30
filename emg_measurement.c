#include <stdio.h>
#include "emg_measurement.h"

adc_meas_t measurement;
adc_meas_queue_t output_queue;
uint8_t slave_pin;

int index = 0;

int queueInsert (adc_meas_t entry)
{
  if (index != output_queue.queue_size - 1)
  {
    output_queue.buffer[index] = entry;
    index++;

    return 0;
  }

  else return -1;
}

bool queueFull (void)
{
  if (index == output_queue.queue_size)
  return true;
  else
  return false;
}

adc_meas_t adcRead (void)
{
  measurement.channel_A_raw = transfer();
  measurement.channel_A = 3.3 * channel_A_raw;
  measurement.channel_B_raw = transfer();
  measurement.channel_B = 3.3 * channel_A_raw;

  return measurement;
}

uint8_t queueReceive (adc_meas_t values[])
{
  int i = 0;

  while (output_queue.buffer[i].channel_A != NULL)
  {
    values[i].channel_A = output_queue.buffer[i].channel_A;
    values[i].channel_B = output_queue.buffer[i].channel_B;
    values[i].channel_A_raw = output_queue.buffer[i].channel_A_raw;
    values[i].channel_B_raw = output_queue.buffer[i].channel_B_raw;

    i++;
  }

return i;  
}
