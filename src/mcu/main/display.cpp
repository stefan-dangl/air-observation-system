#include "pin_layout.h"
#include <SevSeg.h>

// --- constants, defines and global variables ---
#define NUMBER_OF_DISPLAY_DIGITS 3
#define MAX_DISPLAYED_VALUE 10000 - 1
#define DIVIDER 10

SevSeg g_sevseg; 
char g_displayedSensorValue[3];
// --- constants, defines and global variables end ---

void sevsegInit(){
  byte numDigits = 4;   
  byte digitPins[] = {DIGI1, DIGI2, DIGI3, DIGI4}; 
  byte segmentPins[] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_P};
  g_sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  g_sevseg.setBrightness(0); 
}

void displaySelectedSensor(int sensor_id){
  char digit_0[2] = {g_displayedSensorValue[0], '\0'};
  char digit_1[2] = {g_displayedSensorValue[1], '\0'};
  char digit_2[2] = {g_displayedSensorValue[2], '\0'};

  char serialOutput[5] = "";
  sprintf(serialOutput + strlen(serialOutput), "%d", sensor_id + 1);
  sprintf(serialOutput + strlen(serialOutput), ".");
  sprintf(serialOutput + strlen(serialOutput), digit_0);
  sprintf(serialOutput + strlen(serialOutput), digit_1);
  sprintf(serialOutput + strlen(serialOutput), digit_2);

  g_sevseg.blank();
  g_sevseg.setChars(serialOutput);
  g_sevseg.refreshDisplay();
}

void setDisplayedSensorValue(float sensorValue){
    if (sensorValue >= 0 && sensorValue <= MAX_DISPLAYED_VALUE) {
      int displayedNumber = (int)sensorValue / DIVIDER;
      int displayIndex = NUMBER_OF_DISPLAY_DIGITS - 1;

      while (displayedNumber > 0) {
        g_displayedSensorValue[displayIndex] = (displayedNumber % 10) + '0';
        displayedNumber /= 10;
        displayIndex -= 1;
      }
      while(displayIndex >= 0){
        g_displayedSensorValue[displayIndex] = '0';
        displayIndex -= 1;
      }

    } else {
      for (int i = 0; i < NUMBER_OF_DISPLAY_DIGITS; i++){
        g_displayedSensorValue[i] = '-';
      }
    }
  }
