#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#ifndef LED_STRIP_PIN
  #include "pin_layout.h"
#endif

void ledStripInit();
void resetAllLeds();
void displayLevel(int sensor_index, int level);

extern Adafruit_NeoPixel ledStrip;

