#include "MultiShield.h"

#define READ_FREQUENCY_MS 1000
#define SENSOR_PIN A4

unsigned long int then, now;
float temperature;

void setup() {
  // put your setup code here, to run once:
  analogReference (INTERNAL);
  ms_setup ();
  then = millis ();
  temperature = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  now = millis ();
  if (now - then > READ_FREQUENCY_MS) {
    temperature = 317.75 - analogRead (SENSOR_PIN) * 1.25;
    then = now;
  }
  ms_float (temperature, 1);
}
