#include <OneWire.h>
#include <DallasTemperature.h>

#define DALLAS_SENSOR_PIN 4

OneWire oneWire (DALLAS_SENSOR_PIN);
DallasTemperature sensors (&oneWire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  sensors.begin ();
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors.requestTemperatures();
  Serial.println (sensors.getTempCByIndex (0));
}
