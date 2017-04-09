#include "MultiShield.h"

#define TIMER_DELAY_MS  100

unsigned long int then, now;
unsigned int number = 2345;

void setup() {
  // put your setup code here, to run once:
  ms_setup ();
  then = millis ();
  number = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  now = millis ();
  if (now - then > TIMER_DELAY_MS) {
    number = number + 1;
    if (number > 9999)
      number = 0;
    then = now;
  }
  ms_number (number);
}
