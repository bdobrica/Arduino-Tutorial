#define SENSOR_PIN A0

void setup() {
  // put your setup code here, to run once:
  // analogReference(INTERNAL);
  Serial.begin (9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int v = analogRead (SENSOR_PIN);
  Serial.println (v);
}
