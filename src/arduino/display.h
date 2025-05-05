#ifndef SevSeg
  #include <SevSeg.h>
#endif

#ifndef DIGI1
  #include "pin_layout.h"
#endif

#define NUMBER_OF_DISPLAY_DIGITS 3
#define MAX_DISPLAYED_VALUE 10000
#define DIVIDER 10

void sevsegInit();
void displaySelectedSensor(int8_t sensor_id);
void setDisplayedSensorValue(float sensorValue);
