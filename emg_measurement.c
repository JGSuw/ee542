#include <stdio.h>
#include "emg_measurement.h"

#define ADC_SPI_TRANSACTION_SIZE 2

static adc_meas_t measurement;
static adc_meas_t output_queue[BUFFER_SIZE];

static uint8_t slave_pin = 4;

static int buffer_head = 0;

uint8_t adc_read_channel_1 = {byte1, byte2};

uint8_t adc_read_channel_2 = {byte1, byte2}

int queueInsert (adc_meas_t entry)
{
  if (buffer_head < BUFFER_SIZE)
  {
    output_queue[buffer_head++] = entry;
    return 0;
  }

  else return -1;
}

bool queueFull (void)
{
  if (buffer_head == BUFFER_SIZE) {
	  return true;
  }
  else {  
	return false;
  }
}

// TODO
// Helper method called before ADC reads
//
void adcSPIconfig () {
  // Configure slave pin
  // Set slave pin polarity to ENABLE
  // Set data mode of SPI1 interface
  // Set bit order of SPI1 interface
}

adc_meas_t adcRead (void)
{
  // TODO
  adcSPIconfig();
  
  // TODO - implement the SPI protocol used by the ADC
  uint8_t measurement_buffer[2];
  
  transfer (adc_read_channel_1, measurement_buffer, ADC_SPI_TRANSACTION_SIZE, NULL);
  
  // TODO format raw data in measurement buffer
  // Put integer representation and floating representation into measurement structure
  
  transfer (adc_read_channel_2, measurement_buffer, ADC_SPI_TRANSACTION_SIZE, NULL);
  
  // TODO format raw data in measurement buffer
  // Put integer representaiton and floating representation into measurement structure
  
  // TODO set spi slave pin polarity to DISABLE
  return measurement;
}

uint8_t queueReceive (adc_meas_t values[])
{
  // GUARD AGAINST BUFFER OVERFLOW - MAKE SURE VALUES IS THE RIGHT SIZE
  int i;
  for (i = 0; i < buffer_head; i++ ) {
    values[i].channel_A = output_queue.buffer[i].channel_A;
    values[i].channel_B = output_queue.buffer[i].channel_B;
    values[i].channel_A_raw = output_queue.buffer[i].channel_A_raw;
    values[i].channel_B_raw = output_queue.buffer[i].channel_B_raw;
    // TODO copy the data
  }
  buffer_head = 0;

  }

return i;  
}

// TODO actual task implementation
