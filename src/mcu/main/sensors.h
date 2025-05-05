#include <MQ135.h>

#ifndef PIN_MQ135_0
  #include "pin_layout.h"
#endif

void pollAllSensors();
void sendAllSensorValues();
int getCo2Level(int sensor_index);
float getCo2Value(int sensor_index);