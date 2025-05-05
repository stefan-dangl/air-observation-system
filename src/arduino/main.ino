#include "display.h"
#include "led_strip.h"
#include "pin_layout.h"
#include "sensors.h"

// --- constants and global variables --- 
const long intervalButtonBounce = 333;  
const long intervalSensorPoll = 333;  

unsigned long previousMillisButtonBounce = 0;
unsigned long previousMillisSensorPoll = 0;

int8_t displayedSensor = 0;
// --- constants and global variables end --- 

void setup() {
  Serial.begin(9600);
  pin_setup();
  while (!Serial);
  Serial.println("init led strip");
  ledStripInit();
  Serial.println("init 7 segment display");
  sevsegInit();
  Serial.println("Init done");
}

void loop() {
  unsigned long currentMillis = millis();
  displaySelectedSensor(displayedSensor);
  slecetDisplayedSensor(currentMillis);

  if (currentMillis - previousMillisSensorPoll >= intervalSensorPoll) {
    previousMillisSensorPoll = currentMillis;
    pollAllSensors();
    sendAllSensorValues();
    displayAllLevels();
    float sensorValue = getCo2Value(displayedSensor);
    setDisplayedSensorValue(sensorValue);
  }
}

void slecetDisplayedSensor(unsigned long currentMillis){
  if (currentMillis - previousMillisButtonBounce < intervalButtonBounce) {
    return;
  }

  int buttonState_1 = digitalRead(BUTTON_1);
  int buttonState_2 = digitalRead(BUTTON_2);

  if (buttonState_1 && !buttonState_2){
    displayedSensor = (displayedSensor + 1) % NUMBER_OF_SENSORS;
    previousMillisButtonBounce = currentMillis;
  } else if (!buttonState_1 && buttonState_2) {
    displayedSensor = (displayedSensor - 1 + NUMBER_OF_SENSORS) % NUMBER_OF_SENSORS;
    previousMillisButtonBounce = currentMillis;
  }
}

void displayAllLevels(){
  resetAllLeds();
  for (int i = 0; i < NUMBER_OF_SENSORS; i++){
    displayLevel(i, getCo2Level(i));
  }
}
