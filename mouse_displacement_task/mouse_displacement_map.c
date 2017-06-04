#include <stdio.h>
#include <math.h>
#include "mouse_displacement_map.h"
#include "imu_measurement.h"

#define ARRAY_LENGTH 3
#define RANGE 45
#define PI 3.14159265

static float measured_accel[ARRAY_LENGTH];
static float transformed_accel[ARRAY_LENGTH];
static float measured_rotation[ARRAY_LENGTH];
static float transformed_rotation[ARRAY_LENGTH];
static float calculated_centripetal_accel[ARRAY_LENGTH];
static float calculated_pitch_angle;
static float calculated_roll_angle;
static float max_y_displacement; // tune this value
static float max_x_displacement; // tune this value
static uint8_t delta_x;
static uint8_t delta_y;

void mouse_displacement_task() {
	//get measurements
	measured_accel[0] = getX();
	measured_accel[1] = getX();
	measured_accel[2] = getX();
	measured_rotation[0] = getRollRate();
	measured_rotation[1] = getPitchRate();
	measured_rotation[2] = getYawRate();

	coordinateTransformation(measured_accel, measured_rotation);
	// calculateCentripetalAccel(transformed_accel, transformed_rotation);
	resolveAxes(transformed_accel);
	// resolveAxes(calculateCentripetalAccel);
	delta_x = mapXDisplacement();
	delta_y = mapYDisplacement();
  }

void coordinateTransformation(float[] accel, float[] rotate) {
  transformed_accel[0] = accel[1];
  transformed_accel[1] = accel[2];
  transformed_accel[2] = accel[0];

  transformed_rotation[0] = rotate[1];
  transformed_rotation[1] = rotate[2];
  transformed_rotation[2] = rotate[0];
}

/*void calculateCentripetalAccel(float*, float*) {

}*/

void resolveAxes(float[] gravity) {
  calculated_roll_angle = atan(-1 * gravity[0] / gravity[1]) * 180 / PI;
  calculated_pitch_angle = atan(gravity[1] / sqrt(pow(gravity[0],2) + pow(gravity[2],2))) * 180 / PI;
}

void mapXDisplacement() {
	// TODO: Put this into bins, account of deadzone as well.
  delta_x = (uint8_t)(max_x_displacement * calculated_roll_angle / RANGE);
}

void mapYDisplacement() {
	// TODO: put this into bins, account for deadzone as well.
  delta_y = (uint8_t)(max_y_displacement * calculated_pitch_angle / RANGE);
}

int getXDisplacement() {
  return (delta_x);
}

int getYDisplacement() {
  return (delta_y);
}
