#include "pin_layout.h"
#include <Adafruit_NeoPixel.h>

// --- constants, defines and global variables ---
Adafruit_NeoPixel g_ledStrip(NUMBER_OF_SENSOR_GROUPS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
// --- constants, defines and global variables end ---

void ledStripInit()
{
  g_ledStrip.begin();           // INITIALIZE NeoPixel ledStrip object (REQUIRED)
  g_ledStrip.show();            // Turn OFF all pixels ASAP
  g_ledStrip.setBrightness(50); // max = 255
}

static void setLedStrip(int ledIndex, uint32_t color)
{
  g_ledStrip.setPixelColor(ledIndex, color);
  g_ledStrip.show();
}

static void resetLedStrip(int ledIndex)
{
  g_ledStrip.setPixelColor(ledIndex, g_ledStrip.Color(0, 0, 0));
  g_ledStrip.show();
}

void resetAllLeds()
{
  for (int i = 0; i < NUMBER_OF_SENSOR_GROUPS; i++)
  {
    resetLedStrip(i);
  }
}

void displayLevel(int sensorIndex, int level)
{
  switch (level)
  {
  case 0:
    setLedStrip(sensorIndex, g_ledStrip.Color(0, 255, 0));
    break;
  case 1:
    setLedStrip(sensorIndex, g_ledStrip.Color(255, 255, 0));
    break;
  case 2:
    setLedStrip(sensorIndex, g_ledStrip.Color(255, 0, 0));
    break;
  default:
    setLedStrip(sensorIndex, g_ledStrip.Color(0, 0, 255));
  }
  g_ledStrip.show();
}
