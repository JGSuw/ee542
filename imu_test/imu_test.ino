#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif

#include "imu_measurement.h"
#include "stdio.h"

#define TEST_PERIOD 8

#define TEST_SLEEP_MS 100

void setup() {
  // put your setup code here, to run once:
  configureIMU();
  Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  static int read_count = 0;

  imu_measurement_task();
  if (read_count++ % TEST_PERIOD == 0) {
    char buffer[128];
    sprintf( buffer, "x: %f\ty: %f\tz: %f", getX(), getY(), getZ());
    Serial.println( buffer );
  }

  delay(TEST_SLEEP_MS);
}
