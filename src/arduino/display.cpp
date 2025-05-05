#include "display.h"

SevSeg sevseg; 
char displayedSensorValue[3];

void sevsegInit(){
  byte numDigits = 4;   
  byte digitPins[] = {DIGI1, DIGI2, DIGI3, DIGI4}; 
  byte segmentPins[] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_P};
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(0); 
}

void displaySelectedSensor(int8_t sensor_id){
  char digit_0[2] = {displayedSensorValue[0], '\0'};
  char digit_1[2] = {displayedSensorValue[1], '\0'};
  char digit_2[2] = {displayedSensorValue[2], '\0'};

  char serial_output[5] = "";
  sprintf(serial_output + strlen(serial_output), "%d", sensor_id + 1);
  sprintf(serial_output + strlen(serial_output), ".");
  sprintf(serial_output + strlen(serial_output), digit_0);
  sprintf(serial_output + strlen(serial_output), digit_1);
  sprintf(serial_output + strlen(serial_output), digit_2);

  sevseg.blank();
  sevseg.setChars(serial_output);
  sevseg.refreshDisplay();
}

void setDisplayedSensorValue(float sensorValue){
    if (sensorValue >= 0 && sensorValue < MAX_DISPLAYED_VALUE) {
      int displayedInt = (int)sensorValue / DIVIDER;
      int displayIndex = NUMBER_OF_DISPLAY_DIGITS - 1;

      while (displayedInt > 0) {
        displayedSensorValue[displayIndex] = (displayedInt % 10) + 48;
        displayedInt /= 10;
        displayIndex -= 1;
      }
      while(displayIndex >= 0){
        displayedSensorValue[displayIndex] = '0';
        displayIndex -= 1;
      }

    } else {
      for (int i = 0; i < NUMBER_OF_DISPLAY_DIGITS; i++){
        displayedSensorValue[i] = '-';
      }
    }
  }