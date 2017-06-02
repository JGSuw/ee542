#include <stdio.h>
#include "imu_measurement.h"
#include "SparkFunLSM6DS3.h"
#include "SPI.h"


static imu_meas_t imu_avg;
static imu_meas_t measurement; //instance used by read() to store data from LSM6DS3
static imu_meas_queue_t input_queue;
static uint8_t input_buffer_len = IMU_BUFFER_SIZE;
static uint8_t slave_pin = A2;
static int index = 0;
static int i; //loop control variable for averaging funcitons
static float sum = 0.0;

void configureIMU()
{
  //gyroscope settings
  myIMU.settings.gyroEnabled = 1;  //Can be 0 or 1
  myIMU.settings.gyroRange = 2000;   //Max deg/s.  Can be: 125, 245, 500, 1000, 2000
  myIMU.settings.gyroSampleRate = 1666;   //Hz.  Can be: 13, 26, 52, 104, 208, 416, 833, 1666
  myIMU.settings.gyroBandWidth = 200;  //Hz.  Can be: 50, 100, 200, 400;
  myIMU.settings.gyroFifoEnabled = 1;  //Set to include gyro in FIFO
  myIMU.settings.gyroFifoDecimation = 1;  //set 1 for on /1

  //accelerometer settings
  myIMU.settings.accelEnabled = 1;
  myIMU.settings.accelRange = 16;      //Max G force readable.  Can be: 2, 4, 8, 16
  myIMU.settings.accelSampleRate = 833;  //Hz.  Can be: 13, 26, 52, 104, 208, 416, 833, 1666, 3332, 6664, 13330
  myIMU.settings.accelBandWidth = 200;  //Hz.  Can be: 50, 100, 200, 400;
  myIMU.settings.accelFifoEnabled = 1;  //Set to include accelerometer in the FIFO
  myIMU.settings.accelFifoDecimation = 1;  //set 1 for on /1
  myIMU.settings.tempEnabled = 1;

  myIMU.settings.commMode = 1;

  //FIFO control settings
  myIMU.settings.fifoThreshold = 100;  //Can be 0 to 4096 (16 bit bytes)
  myIMU.settings.fifoSampleRate = 50;  //Hz.  Can be: 10, 25, 50, 100, 200, 400, 800, 1600, 3300, 6600
  myIMU.settings.fifoModeWord = 6;  //FIFO mode.

  //FIFO mode.  Can be:
  //  0 (Bypass mode, FIFO off)
  //  1 (Stop when full)
  //  3 (Continuous during trigger)
  //  4 (Bypass until trigger)
  //  6 (Continous mode)
}

void read()
{
  // Configure slave pin
  SPI1.begin(SPI_MODE_SLAVE, slave_pin);
  // Set bit order for SPI1 interface
  SPI1.setBitOrder(MSBFIRST);
  // Set data mode of SPI1 interface
  SPI1.setDataMode(SPI_MODE3);

  configureIMU();
  myIMU.fifoBegin();

  while(( myIMU.fifoGetStatus() & 0x8000 ) == 0 ) {};  //Wait for watermark

  //We loop until FIFO is empty.  NOTE:  As the FIFO is only 8 bits wide,
  //the channels must be synchronized to a known position for the data to align
  //properly.  Emptying the fifo is one way of doing this.

  while( ( myIMU.fifoGetStatus() & 0x1000 ) == 0 )
  {
    measurement.gyro[0] = myIMU.calcGyro(myIMU.fifoRead());
    measurement.gyro[1] = myIMU.calcGyro(myIMU.fifoRead());
    measurement.gyro[2] = myIMU.calcGyro(myIMU.fifoRead());
    measurement.accel[0] = myIMU.calcAccel(myIMU.fifoRead());
    measurement.accel[1] = myIMU.calcAccel(myIMU.fifoRead());
    measurement.accel[2] = myIMU.calcAccel(myIMU.fifoRead());

    delay(10); //Wait for the serial buffer to clear (~50 bytes worth of time @ 57600baud)
  }

  myIMU.fifoClear();
}

void averageReadings()
{
  //Store average values for accelaeration
  imu_avg.accel[0] = getX();
  imu_avg.accel[1] = getY();
  imu_avg.accel[2] = getZ();

  //Store average value for angluar velocities
  imu_avg.gyro[0] = getRollRate();
  imu_avg.gyro[1] = getPitchRate();
  imu_avg.gyro[2] = getYawRate();
}

float getX()
{
  sum = 0.0;

  for(i = 0; i < input_buffer_len; i++)
  sum+= input_queue.buffer[i].accel[0];

  return (sum / input_buffer_len);
}

float getY()
{
  sum = 0.0;

  for(i = 0; i < input_buffer_len; i++)
  sum+= input_queue.buffer[i].accel[1];

  return (sum / input_buffer_len);
}

float getZ()
{
  sum = 0.0;

  for(i = 0; i < input_buffer_len; i++)
  sum+= input_queue.buffer[i].accel[2];

  return (sum / input_buffer_len);
}

float getRollRate()
{
  sum = 0.0;

  for(i = 0; i < input_buffer_len; i++)
  sum+= input_queue.buffer[i].gyro[0];

  return (sum / input_buffer_len);
}

float getPitchRate()
{
  sum = 0.0;

  for(i = 0; i < input_buffer_len; i++)
  sum+= input_queue.buffer[i].gyro[1];

  return (sum / input_buffer_len);
}

float getYawRate()
{
  sum = 0.0;

  for(i = 0; i < input_buffer_len; i++)
  sum+= input_queue.buffer[i].gyro[2];

  return (sum / input_buffer_len);
}
