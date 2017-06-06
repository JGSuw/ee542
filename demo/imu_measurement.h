#ifndef IMU_HEADER
#define IMU_HEADER

#define ARRAY_SIZE 3
#define IMU_BUFFER_SIZE 8

typedef struct {
  float accel[ARRAY_SIZE];
  float gyro[ARRAY_SIZE];
} imu_meas_t;

void imu_measurement_task();
void configureIMU();
float getX();
float getY();
float getZ();
float getYawRate();
float getPitchRate();
float getRollRate();

#endif

