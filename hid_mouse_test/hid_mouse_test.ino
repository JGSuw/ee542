#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif

STARTUP(Mouse.begin());

#define MOUSE_UPDATE_PERIOD_MS 8

#define FREQUENCY_RAD_S 6.28
#define DELTA FREQUENCY_RAD_S * MOUSE_UPDATE_PERIOD_MS / 1000

#define SCREEN_SIZE_X 1920
#define SCREEN_SIZE_Y 1080

#define CIRCLE_RADIUS 300

void setup() {
  // put your setup code here, to run once:
  Mouse.screenSize(1920,1080);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  static float theta = 0.0;
  int x = CIRCLE_RADIUS * cos(theta);
  int y = CIRCLE_RADIUS * sin(theta);
  theta = theta + DELTA;
  Mouse.moveTo(SCREEN_SIZE_X / 2 + x, SCREEN_SIZE_Y / 2 + y);
  delay(MOUSE_UPDATE_PERIOD_MS);
}
