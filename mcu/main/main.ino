#include <SevSeg.h>
#include "pin_layout.h"
#include "display.h"
#include "led_strip.h"

// --- constants and global variables --- 
const long intervalButtonBounce = 333;  // interval at which to blink (milliseconds)
const long intervalSensorPoll = 333;  // interval at which to blink (milliseconds)

unsigned long previousMillisButtonBounce = 0;
unsigned long previousMillisSensorPoll = 0;
int8_t displayedSensor = 0;
// --- constants and global variables end --- 

void setup() {
  Serial.begin(9600);
  pin_setup();
  Serial.println("init led strip");
  ledStripInit();
  Serial.println("init 7 segment display");
  sevsegInit();
  Serial.println("Init done");
}

void loop() {

  // unsigned long currentMillis = millis();
  // displaySelectedSensor(displayedSensor);
  // slecetDisplayedSensor(currentMillis);

  // if (currentMillis - previousMillisSensorPoll >= intervalSensorPoll) {
  //   previousMillisSensorPoll = currentMillis;
  //   // cache current sensor values

  //   // pollSensor();
 
  //   // check if sensor value changed
  //     // send via serial
  //     // check if led strip has to be adapted
  // }

  for (int i = 0; i < 100; i++){
    sendDummyData(i);
    delay(1000); 
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






// --- examples ---
void buttonsAndSerialOutputExample(){
  int buttonState_1 = digitalRead(BUTTON_1);
  int buttonState_2 = digitalRead(BUTTON_2);

  String serial_output = "Values: ";
  serial_output += buttonState_1;
  serial_output += ";";
  serial_output += buttonState_2;
  serial_output += ";";

  Serial.println(serial_output);
  delay(1); 
}

void ledStripExample(){
  resetLedStrip(2);
  setLedStrip(0, ledStrip.Color(255,   0,   0));
  delay(1000);
  resetLedStrip(0);
  setLedStrip(1, ledStrip.Color(0,   255,   0));
  delay(1000);
  resetLedStrip(1);
  setLedStrip(2, ledStrip.Color(0,   0,   255));
  delay(1000);
}

void sendDummyData(int sensor_id){

  char buffer[100];
  sprintf(buffer, "0;%i.23;%i.34;%i.45;%i.56;%i.67", sensor_id, sensor_id, sensor_id, sensor_id, sensor_id, sensor_id);
  Serial.println(buffer);
}

// --- examples end ---