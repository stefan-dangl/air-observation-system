/*
  Copyright (c) 2025 Stefan Dangl.  All right reserved.

 This project is based on the STM32F334 Nucleo Board and utilizes the 
 Arduino Framework. MQ135 gas sensors are used for measuring CO2 and MQ2 
 sensors for measuring CH4 (methane). In the current implementation two 
 sensor pairs are defined. Further sensors can be added easily. The sensor 
 data is outputted via serial interface. Additionally an Adafruit NeoPixel 
 for showing the current CO2 level (normal, high, critical) and a 3641AS 
 seven segment display for outputting the current CO2 value is used. The 
 seven segment display is limited to 4 digits. Accordingly, buttons are 
 used to switch between the sensor pairs.
*/

#include "display.h"
#include "led_strip.h"
#include "pin_layout.h"
#include "sensors.h"

// --- constants, defines and global variables ---
const long intervalButtonBounce = 333;    // milliseconds
const long intervalSensorPoll = 500;      // milliseconds

unsigned long g_previousMillisButtonBounce = 0;
unsigned long g_previousMillisSensorPoll = 0;

int g_displayedSensor = 0;
// --- constants, defines and global variables end ---

void setup()
{
  Serial.begin(9600);
  pin_setup();
  while (!Serial)
    ;
  Serial.println("init sensors");
  initAllSensors();
  Serial.println("init led strip");
  ledStripInit();
  Serial.println("init 7 segment display");
  sevsegInit();
  Serial.println("Init done");
}

void loop()
{
  unsigned long currentMillis = millis();
  slecetDisplayedSensor(currentMillis);
  displaySelectedSensor(g_displayedSensor);

  if (currentMillis - g_previousMillisSensorPoll >= intervalSensorPoll)
  {
    g_previousMillisSensorPoll = currentMillis;
    pollAllSensors();
    sendAllSensorValues();
    displayAllLevels();
    setDisplayedSensorValue(getCo2Value(g_displayedSensor));
  }
}

void slecetDisplayedSensor(unsigned long currentMillis)
{
  if (currentMillis - g_previousMillisButtonBounce < intervalButtonBounce)
  {
    return;
  }

  int buttonState_1 = digitalRead(BUTTON_1);
  int buttonState_2 = digitalRead(BUTTON_2);

  if (buttonState_1 && !buttonState_2)
  {
    g_displayedSensor = (g_displayedSensor + 1) % NUMBER_OF_SENSOR_GROUPS;
    g_previousMillisButtonBounce = currentMillis;
  }
  else if (!buttonState_1 && buttonState_2)
  {
    g_displayedSensor = (g_displayedSensor - 1 + NUMBER_OF_SENSOR_GROUPS) % NUMBER_OF_SENSOR_GROUPS;
    g_previousMillisButtonBounce = currentMillis;
  }
}

void displayAllLevels()
{
  resetAllLeds();
  for (int i = 0; i < NUMBER_OF_SENSOR_GROUPS; i++)
  {
    displayLevel(i, getCo2Level(i));
  }
}
