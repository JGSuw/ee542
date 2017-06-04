#if defined(ARDUINO)
SYSTEM_MODE(SEMI_AUTOMATIC);
#endif

STARTUP(Mouse.begin());

#include "mouse_displacement_map.h"
#include "imu_measurement.h"
#include "stdio.h"
#include "hw_ticks.h"

#define UPDATE_PERIOD_US 8000
#define SAMPLE_PERIOD_US 1000
#define SCREEN_SIZE_X 1920
#define SCREEN_SIZE_Y 1080


void hid_mouse_task_initialize(void);
void hid_mouse_task(void);

void hid_mouse_task_initialize(void) {
  Mouse.screenSize(SCREEN_SIZE_X, SCREEN_SIZE_Y);
}

void hid_mouse_task(void) {
  //bool lmb = getLMB();
  //bool rmb = getRMB();
  int delta_x = getXDisplacement();
  int delta_y = getYDisplacement();
  
  Mouse.move(delta_x, delta_y);

  /*if (lmb){ 
    Mouse.press(MOUSE_LEFT);
  } else {
    Mouse.release(MOUSE_LEFT);
  }

  if (rmb){
    Mouse.press(MOUSE_RIGHT);
  } else {
    Mouse.release(MOUSE_RIGHT);
  } */
}

void setup() {
  // put your setup code here, to run once:
  configureIMU();
  hid_mouse_task_initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  static system_tick_t tick = GetSystem1UsTick();
  static system_tick_t tock = tick;
  
  system_tick_t now = GetSystem1UsTick();
  if ( now - tick > SAMPLE_PERIOD_US) {
    imu_measurement_task();
    if (now - tock > UPDATE_PERIOD_US) {
      mouseDisplacementTask();
      hid_mouse_task();
      tock = now;
    }
    tick = now;
  }
}
