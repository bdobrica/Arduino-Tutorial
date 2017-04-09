#include "MultiShield.h"

#define WAIT_FOR_MS   200

unsigned long int then, now;
int number;
bool wait;

void setup() {
  // put your setup code here, to run once:
  ms_setup ();
  number = 0;
  wait = false;
  then = millis ();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!wait) {
    if (ms_keypress (0)) {
      number ++;
      wait = true;
      then = millis ();
      }
    if (ms_keypress (1)) {
      number --;
      wait = true;
      then = millis ();
      }
    if (ms_keypress (2)) {
      number = 0;
      wait = true;
      then = millis ();
      }
    if (number < 0) {
      number = 0;
    }
    if (number > 9999) {
      number = 9999;
    }
  }
  else {
    now = millis ();
    if (now - then > WAIT_FOR_MS) {
      wait = false;
    }
  }
  ms_number (number);
}
