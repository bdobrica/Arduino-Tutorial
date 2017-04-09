#include <OneWire.h>
#include <DallasTemperature.h>

#include "MultiShield.h"

#define DALLAS_SENSOR_PIN A4
#define READ_FREQUENCY_MS  3000

OneWire oneWire (DALLAS_SENSOR_PIN);
DallasTemperature sensors (&oneWire);

float temperature;
unsigned long int then, now;

void setup() {
  // put your setup code here, to run once:
  sensors.begin ();
  sensors.setResolution (9);
  sensors.requestTemperatures ();
  temperature = sensors.getTempCByIndex (0);
  ms_setup ();
  then = millis ();
}

void loop() {
  // put your main code here, to run repeatedly:
  now = millis ();
  if (now - then > READ_FREQUENCY_MS) {
    sensors.requestTemperatures ();
    temperature = sensors.getTempCByIndex(0);
    then = now;
    }
  ms_float (temperature, 2);
}
