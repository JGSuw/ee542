#include <stdio.h>
#include "emg_measurement.h"
#include "Particle.h"

#define ADC_SPI_TRANSACTION_SIZE 2

static adc_meas_t measurement;
static adc_meas_queue_t output_queue;
static uint8_t slave_pin = A2;
static int index = 0;

static uint8_t adc_read_channel_A[] = {0xFF, 0xFF};
static uint8_t adc_read_channel_B[] = {0xFF, 0xFF};

int queueInsert (adc_meas_t entry)
{
  if (index < output_queue.queue_size)
  {
    output_queue.buffer[index++] = entry;
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

// Helper method called before ADC reads
void adcSPIconfig ()
{
  // Configure slave pin
  SPI1.begin(SPI_MODE_SLAVE, slave_pin);
  // Set slave pin polarity to ENABLE


  // Set bit order for SPI1 interface
  SPI1.setBitOrder(MSBFIRST);
  // Set data mode of SPI1 interface
  SPI1.setDataMode(SPI_MODE3);
}

adc_meas_t adcRead (void)
{
  adcSPIconfig();

  // TODO - implement the SPI protocol used by the ADC
  uint8_t measurement_buffer[2];

  transfer (adc_read_channel_A, measurement_buffer, ADC_SPI_TRANSACTION_SIZE, NULL);
  measurement.channel_A_raw = (measurement_buffer[0] << 8) | (measurement_buffer[1] & 0xff);
  measurement.channel_A = 3.3 * measurement.channel_A_raw;

  transfer (adc_read_channel_B, measurement_buffer, ADC_SPI_TRANSACTION_SIZE, NULL);
  measurement.channel_B_raw = (measurement_buffer[0] << 8) | (measurement_buffer[1] & 0xff);
  measurement.channel_B = 3.3 * measurement.channel_B_raw;

  // TODO set spi slave pin polarity to DISABLE
  SPI1.end();
  return measurement;
}

uint8_t queueReceive (adc_meas_t values[]) //returns -1 if there is a mismatch in array sizes
{
  // GUARD AGAINST BUFFER OVERFLOW - MAKE SURE VALUES IS THE RIGHT SIZE
  if (sizeof(values) == output_queue.queue_size)
  {
    int i;
    for (i = 0; i < index; i++ ) {
      values[i].channel_A = output_queue.buffer[i].channel_A;
      values[i].channel_B = output_queue.buffer[i].channel_B;
      values[i].channel_A_raw = output_queue.buffer[i].channel_A_raw;
      values[i].channel_B_raw = output_queue.buffer[i].channel_B_raw;
    }
    index = 0;
    return i;
  }
  else
    return -1;
}

// TODO actual task implementation
