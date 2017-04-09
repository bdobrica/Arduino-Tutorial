#include <OneWire.h>
#include <DallasTemperature.h>

#define DALLAS_SENSOR_PIN 4
#define SENSOR_PIN A0

OneWire oneWire (DALLAS_SENSOR_PIN);
DallasTemperature sensors (&oneWire);

void setup() {
  // put your setup code here, to run once:
  analogReference(INTERNAL);
  Serial.begin (9600);
  sensors.begin ();
}

void loop() {
  // put your main code here, to run repeatedly:
  int v = analogRead (SENSOR_PIN);
  sensors.requestTemperatures ();
  float t = sensors.getTempCByIndex (0);
  Serial.print (v);
  Serial.print ('\t');
  Serial.println (t);
}
