#ifndef IMU_HEADER
#define IMU_HEADER

#define ARRAY_SIZE 3
#define IMU_BUFFER_SIZE 8

typedef struct imu_meas_t{
  float accel[ARRAY_SIZE];
  float gyro[ARRAY_SIZE];
};

typedef struct imu_meas_queue_t{
  const uint8_t queue_size = IMU_BUFFER_SIZE;
  imu_meas_t buffer[IMU_BUFFER_SIZE];
};

void configureIMU();
float getX();
float getY();
float getZ();
float getYawRate();
float getPitchRate();
float getRollRate();

#endif
