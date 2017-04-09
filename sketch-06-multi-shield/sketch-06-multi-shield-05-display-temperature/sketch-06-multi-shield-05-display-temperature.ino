#include <OneWire.h>
#include <DallasTemperature.h>

#include "MultiShield.h"

#define DALLAS_SENSOR_PIN A4

OneWire oneWire (DALLAS_SENSOR_PIN);
DallasTemperature sensors (&oneWire);

void setup() {
  // put your setup code here, to run once:
  sensors.begin ();
  ms_setup ();
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors.requestTemperatures ();
  float temperature = sensors.getTempCByIndex(0);
  ms_float (temperature, 2);
}
