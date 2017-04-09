#include "MultiShield.h"

#define TIMER_DELAY_MS  10

unsigned long int then, now;
float number;

void setup() {
  // put your setup code here, to run once:
  ms_setup ();
  then = millis ();
  number = 0.00;
}

void loop() {
  // put your main code here, to run repeatedly:
  now = millis ();
  if (now - then > TIMER_DELAY_MS) {
    number = number + 0.01;
    if (number > 99.99)
      number = -9.99;
    then = now;
  }
  ms_float (number, 3);
}
