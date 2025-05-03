#include "pin_layout.h"
#include <Arduino.h>

void pin_setup(){
  pinMode(LED_STRIP_PIN, OUTPUT);

  pinMode(BUTTON_1, INPUT_PULLDOWN);
  pinMode(BUTTON_2, INPUT_PULLDOWN);

  pinMode(DIGI1, OUTPUT);
  pinMode(DIGI2, OUTPUT);
  pinMode(DIGI3, OUTPUT);
  pinMode(DIGI4, OUTPUT);

  pinMode(SEGMENT_A, OUTPUT);
  pinMode(SEGMENT_B, OUTPUT);
  pinMode(SEGMENT_C, OUTPUT);
  pinMode(SEGMENT_D, OUTPUT);
  pinMode(SEGMENT_E, OUTPUT);
  pinMode(SEGMENT_F, OUTPUT);
  pinMode(SEGMENT_G, OUTPUT);
  pinMode(SEGMENT_P, OUTPUT);

  pinMode(PIN_MQ135_0, INPUT);
  pinMode(PIN_MQ135_1, INPUT);
}
