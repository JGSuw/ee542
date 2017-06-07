#include <stdio.h>
#include <stdbool.h>
#include "emg_measurement.h"
#include "button_state_mapping.h"

#define BUFFER_SIZE 50
#define CHANNEL_A  0
#define CHANNEL_A_RAW  1
#define CHANNEL_B  2
#define CHANNEL_B_RAW  3

static uint8_t buffer_head;
static bool button_state[2];
static adc_meas_t input_buffer[BUFFER_SIZE];
static uint16_t threshold[2] = {250, 250};

bool defineButtonState(uint16_t, uint16_t);

void updateBuffer(){
  adc_meas_t temp[8];
  int count = 0;
  
  count = adcQueueReceive(temp);    
  
  for (int i = 0; i < count; i++) {
    input_buffer[buffer_head] = temp[i];
    buffer_head = (buffer_head + 1) % BUFFER_SIZE;
  }
}

uint16_t averageChannel(int channel){// channel can be 1 (channel A raw) or 3 (channel B raw)
  uint16_t average;
  uint16_t sum;
  
  average = 0;
  sum = 0;

  for (int i = 0; i < BUFFER_SIZE; i++){
    if (CHANNEL_A_RAW == channel){
      sum += input_buffer[i].channel_A_raw;
    }
    else if (CHANNEL_B_RAW == channel){
      sum += input_buffer[i].channel_B_raw;
    }
  }
  average = sum / BUFFER_SIZE;
  
  return average;
}

void button_state_mapping_task(){
  uint16_t average_channel_A;
  uint16_t average_channel_B;

  updateBuffer();
    
  average_channel_A = averageChannel(1);
  average_channel_B = averageChannel(2);

  button_state[0] = defineButtonState(average_channel_A, threshold[0]);
  button_state[1] = defineButtonState(average_channel_B, threshold[1]);
}

bool defineButtonState(uint16_t average_channel, uint16_t threshold){
  bool button_state;
  
  if (average_channel > threshold){
    button_state = true;
  }
  else {
    button_state = false;
  }  
  return button_state;  
}

void initializedBuffer(){
  for (int i = 0; i < BUFFER_SIZE; i++){
    input_buffer[i].channel_A = 0;
    input_buffer[i].channel_A_raw = 0;
    input_buffer[i].channel_B = 0;
    input_buffer[i].channel_B_raw = 0;
  } 
}

bool getLMB(){ // channel B
  return button_state[0];
}

bool getRMB(){ // channel A
  return button_state[1];
}

void debug_buffer(adc_meas_t * buff) {
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buff[i] = input_buffer[i];
  }
}


