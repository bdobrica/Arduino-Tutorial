#include "MultiShield.h"

int number;

void setup() {
  // put your setup code here, to run once:
  ms_setup ();
  number = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (ms_keypress (0)) number ++;
  if (ms_keypress (1)) number --;
  if (ms_keypress (2)) number = 0;
  if (number < 0) number = 0;
  if (number > 9999) number = 9999;
  ms_number (number);
}
