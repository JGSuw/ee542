#include <stdio.h>
#include <math.h>
#include "mouse_displacement_map.h"
#include "imu_measurement.h"

#define ARRAY_LENGTH 3
#define RANGE 45
#define PI 3.14159265
#define DISPLACEMENT_QUANTIZATION_INTERVAL 3

static float measured_accel[ARRAY_LENGTH];
static float transformed_accel[ARRAY_LENGTH];
static float measured_rotation[ARRAY_LENGTH];
static float transformed_rotation[ARRAY_LENGTH];
static float calculated_centripetal_accel[ARRAY_LENGTH];
static float calculated_pitch_angle;
static float calculated_roll_angle;
static const int max_y_displacement = 15; // tune this value
static const int max_x_displacement = 15; // tune this value
static float delta_x;
static float delta_y;

void coordinateTransformation(float *, float *);
void resolveAxes(float *);
void mapXDisplacement();
void mapYDisplacement();

void mouseDisplacementTask() {
  //get measurements
  measured_accel[0] = getX();
  measured_accel[1] = getY();
  measured_accel[2] = getZ();
  measured_rotation[0] = getRollRate();
  measured_rotation[1] = getPitchRate();
  measured_rotation[2] = getYawRate();

  //transform coordinates
  coordinateTransformation(measured_accel, measured_rotation);
  // calculateCentripetalAccel(transformed_accel, transformed_rotation);
  resolveAxes(transformed_accel);// resolveAxes(calculateCentripetalAccel);
  mapXDisplacement();
  mapYDisplacement();
}


void coordinateTransformation(float * accel, float * rotate) {
  transformed_accel[0] = accel[0];
  transformed_accel[1] = accel[2];
  transformed_accel[2] = accel[1];

  transformed_rotation[0] = rotate[0];
  transformed_rotation[1] = rotate[2];
  transformed_rotation[2] = rotate[1];
}

/*void calculateCentripetalAccel(float*, float*) {

}*/

void resolveAxes(float * gravity) {
  calculated_roll_angle = atan(-1 * gravity[0] / gravity[2]) * 180 / PI;
  calculated_pitch_angle = atan(gravity[1] / sqrt(pow(gravity[0],2) + pow(gravity[2],2))) * 180 / PI;
}

void mapYDisplacement() {
  delta_y = (max_x_displacement * calculated_roll_angle / RANGE);
}

void mapXDisplacement() {
  delta_x = (max_y_displacement * calculated_pitch_angle / RANGE);
}

int getXDisplacement() {
  // TODO put the delta_x into bins
  return ((int)(delta_x / DISPLACEMENT_QUANTIZATION_INTERVAL)*DISPLACEMENT_QUANTIZATION_INTERVAL);
}

int getYDisplacement() {
  // TODO put the delta_y into bins
  return ((int)(delta_y / DISPLACEMENT_QUANTIZATION_INTERVAL)*DISPLACEMENT_QUANTIZATION_INTERVAL);
}
