#include <stdbool.h>
#include <Mouse.h>
#include "hid_mouse_task.h"

#define MOUSE_UPDATE_PERIOD_MS 8
#define SCREEN_SIZE_X 1920
#define SCREEN_SIZE_Y 1080

void hid_mouse_task_initialize(void) {
  Mouse.screenSize(SCREEN_SIZE_X, SCREEN_SIZE_Y);
}

void hid_mouse_task(void) {
  bool lmb = getLMB();
  bool rmb = getRMB();
  int delta_x = getXDisplacement();
  int dleya_y = getYDisplacement();
  
  Mouse.move(delta_x, delta_y);

  if (lmb){ 
    Mouse.press(MOUSE_LEFT);
  } else {
    Mouse.release(MOUSE_LEFT);
  }

  if (rmb){
    Mouse.press(MOUSE_RIGHT);
  } else {
    Mouse.release(MOUSE_RIGHT);
  }
}

