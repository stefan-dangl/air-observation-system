#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#ifndef LED_STRIP_PIN
  #include "pin_layout.h"
#endif

void ledStripInit();
void setLedStrip(int led_id, uint32_t color);
void resetLedStrip(int i);

extern Adafruit_NeoPixel ledStrip;