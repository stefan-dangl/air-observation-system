#include "led_strip.h"


Adafruit_NeoPixel ledStrip(NUMBER_OF_SENSORS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

void ledStripInit(){
  ledStrip.begin();           // INITIALIZE NeoPixel ledStrip object (REQUIRED)
  ledStrip.show();            // Turn OFF all pixels ASAP
  ledStrip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void setLedStrip(int led_id, uint32_t color){
  ledStrip.setPixelColor(led_id, color);
  ledStrip.show();
}

void resetLedStrip(int i){
  ledStrip.setPixelColor(i, ledStrip.Color(0,   0,   0));
  ledStrip.show();
}