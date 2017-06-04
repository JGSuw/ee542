#include <stdio.h>
#include <stdbool.h>
#include "emg_measurement.h"
#include "spi.h"

#define ADC_REF_VOLTAGE 3.3f
#define ADC_RESOLUTION  1024
#define ADC_SPI_TRANSACTION_SIZE 2
#define ADC_SLAVE_ENABLE LOW

static adc_meas_t measurement;
static adc_meas_t output_queue[ADC_BUFFER_SIZE];
static uint8_t slave_pin = 4;
static int buffer_head = 0;
static uint8_t adc_read_channel_A[] = {0b01101000, 0x00};
static uint8_t adc_read_channel_B[] = {0b01111000, 0x00};

int adcQueueInsert (adc_meas_t entry) {
  if (buffer_head < ADC_BUFFER_SIZE)
  {
    output_queue[buffer_head++] = entry;
    return 0;
  }

  else return -1;
}

bool adcQueueFull (void) {
  if (buffer_head == ADC_BUFFER_SIZE) {
    return true;
  }
  else {  
  return false;
  }
}

// Helper method called before ADC reads
void adcSPIconfig () {
  pinMode(4, OUTPUT);
  SPI1.setBitOrder(MSBFIRST);
  SPI1.setDataMode(SPI_MODE3);
}

adc_meas_t adcRead (void) {
  adcSPIconfig();

  digitalWrite(slave_pin, ADC_SLAVE_ENABLE);
  uint8_t measurement_buffer[2];

  SPI1.transfer (adc_read_channel_A, measurement_buffer, ADC_SPI_TRANSACTION_SIZE, NULL);
  measurement.channel_A_raw = (measurement_buffer[0] & 0x03) | (measurement_buffer[1]);
  measurement.channel_A = measurement.channel_A_raw * (ADC_REF_VOLTAGE / ADC_RESOLUTION);

  SPI1.transfer (adc_read_channel_B, measurement_buffer, ADC_SPI_TRANSACTION_SIZE, NULL);
  measurement.channel_B_raw = (measurement_buffer[0] & 0x03) | (measurement_buffer[1]);
  measurement.channel_B = measurement.channel_B_raw * (ADC_REF_VOLTAGE / ADC_RESOLUTION);

  digitalWrite(slave_pin, !ADC_SLAVE_ENABLE);
  return measurement;
}

uint8_t adcQueueReceive (adc_meas_t values[]) {
  int i;
  for (i = 0; i < buffer_head; i++ ) {
    values[i].channel_A = output_queue[i].channel_A;
    values[i].channel_B = output_queue[i].channel_B;
    values[i].channel_A_raw = output_queue[i].channel_A_raw;
    values[i].channel_B_raw = output_queue[i].channel_B_raw;
  }
  buffer_head = 0;
  return i;
}

// Task implementaiton
void emg_measurement_task() {
  // TODO if its time to run
    // sample the channels and put them in the queue
  if (true) {
    measurement = adcRead();
    adcQueueInsert(measurement);
  }
}

