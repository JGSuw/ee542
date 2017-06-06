#include <stdio.h>
#include "imu_measurement.h"
#include "spi.h"
#include <SparkFunLSM6DS3.h>

#define TASK_PERIOD_US  1000


static imu_meas_t imu_avg;
static imu_meas_t measurement; //instance used by read() to store data from LSM6DS3
static imu_meas_t input_queue[IMU_BUFFER_SIZE];
static uint8_t slave_pin = 5;
static int buffer_head = 0;

static LSM6DS3 myIMU = LSM6DS3 ( SPI_MODE, slave_pin );

// Helper function prototypes
float calculatGyroAverages(int);
float calculatAccelAverages(int);
void read(void);
void averageReadings();

void configureIMU()
{
  //gyroscope settings
  myIMU.settings.gyroEnabled = 1;  //Can be 0 or 1
  myIMU.settings.gyroRange = 125;   //Max deg/s.  Can be: 125, 245, 500, 1000, 2000
  myIMU.settings.gyroSampleRate = 1666;   //Hz.  Can be: 13, 26, 52, 104, 208, 416, 833, 1666
  myIMU.settings.gyroBandWidth = 200;  //Hz.  Can be: 50, 100, 200, 400;

  myIMU.settings.accelEnabled = 1;
  //accelerometer settings
  myIMU.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16
  myIMU.settings.accelSampleRate = 833;  //Hz.  Can be: 13, 26, 52, 104, 208, 416, 833, 1666, 3332, 6664, 13330
  myIMU.settings.accelBandWidth = 200;  //Hz.  Can be: 50, 100, 200, 400;
  myIMU.settings.accelFifoEnabled = 0;  //Set to include accelerometer in the FIFO
  myIMU.settings.tempEnabled = 1;

  myIMU.settings.commMode = 1;
  
  myIMU.begin();
}

void imu_measurement_task() {
  read();
  if (buffer_head == IMU_BUFFER_SIZE) {
    averageReadings();
  }
}

void read()
{
  // Configure slave pin
  SPI1.begin(slave_pin);
  // Set bit order for SPI1 interface
  SPI1.setBitOrder(MSBFIRST);
  // Set data mode of SPI1 interface
  SPI1.setDataMode(SPI_MODE3);

  measurement.accel[0] = myIMU.readFloatAccelX();
  measurement.accel[1] = myIMU.readFloatAccelY();
  measurement.accel[2] = myIMU.readFloatAccelZ();
  measurement.gyro[0] = myIMU.readFloatGyroX();
  measurement.gyro[1] = myIMU.readFloatGyroY();
  measurement.gyro[2] = myIMU.readFloatGyroZ();

  if (buffer_head < IMU_BUFFER_SIZE) {
    input_queue[buffer_head++] = measurement;
  }
}

void averageReadings()
{
  imu_avg.accel[0] = calculatAccelAverages(0);
  imu_avg.accel[1] = calculatAccelAverages(1);
  imu_avg.accel[2] = calculatAccelAverages(2);

  //Store average value for angluar velocities
  imu_avg.gyro[0] = calculatGyroAverages(0);
  imu_avg.gyro[1] = calculatGyroAverages(1);
  imu_avg.gyro[2] = calculatGyroAverages(2);

  buffer_head = 0;
}

float calculatAccelAverages(int axis) {
  float sum = 0.0;

  for(int i = 0; i < IMU_BUFFER_SIZE; i++) {
    sum+= input_queue[i].accel[axis];
  }
  
  return (sum / IMU_BUFFER_SIZE);
}

float calculatGyroAverages(int axis) {
  float sum = 0.0;

  for(int i = 0; i < IMU_BUFFER_SIZE; i++) {
    sum+= input_queue[i].gyro[axis];
  }
  
  return (sum / IMU_BUFFER_SIZE);
}

float getX()
{
  return imu_avg.accel[0];
}

float getY()
{
  return imu_avg.accel[1];
}

float getZ()
{
  return imu_avg.accel[2];
}

float getRollRate()
{
  return imu_avg.gyro[0];
}

float getPitchRate()
{
  return imu_avg.gyro[1];
}

float getYawRate()
{
  return imu_avg.gyro[2];
}

