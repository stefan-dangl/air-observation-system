#include "display.h"

SevSeg sevseg; 

void sevsegInit(){
  byte numDigits = 4;   
  byte digitPins[] = {DIGI1, DIGI2, DIGI3, DIGI4}; 
  byte segmentPins[] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_P};
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(0); 
}

void displaySelectedSensor(int8_t sensor_id){
  char serial_output[5] = "";

  sprintf(serial_output + strlen(serial_output), "%d", sensor_id + 1);
  sprintf(serial_output + strlen(serial_output), ".");
  sprintf(serial_output + strlen(serial_output), "5");
  sprintf(serial_output + strlen(serial_output), "1");
  sprintf(serial_output + strlen(serial_output), "9");

  sevseg.blank();
  sevseg.setChars(serial_output);
  sevseg.refreshDisplay();
}