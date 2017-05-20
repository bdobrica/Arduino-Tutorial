#define SENSOR_PIN  A0
#define SOURCE_PIN  3

#define THRESHOLD   1.50
#define SAMPLES     100

#define DELAY       1000

/*
 * The equation governing the wire behaviour is:
 * I^2 R0 (1 + C (Tw - Ta)) = (A + B v^n) (Tw - Ta),
 * where:
 *  I = the current passing the wire
 *  R0 = the wire initial resistance
 *    -> Rw = R0 (1 + C (Tw - Ta)) is the wire resistance
 *  C = wire thermal coeff
 *  Tw = wire temperature
 *  Ta = ambient temperature
 *  A, B, n = heat transfer constants, depend on material and
 *            wire geometry
 *  To improve measurement, let I be constant.
 *
 * Connection:
 *
 *	SOURCE_PIN <---[R]---+---[B]---| GND
 *						 |
 *					 SENSOR_PIN
 */

int voltage = 255;
long now, then;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  analogReference (INTERNAL);
  pinMode (SOURCE_PIN, OUTPUT);
  then = millis ();
}

void loop() {
  // put your main code here, to run repeatedly:
  int c;
  long u = 0;
  float uw = 0, us = 0, ur = 0;

  // Integrating the voltage across the wire
  for (c = 0; c < SAMPLES; c++)
    u += analogRead (SENSOR_PIN);

  uw = 0.0025 * u / SAMPLES;
  us = 0.0195 * voltage;
  ur = us - uw;

  // Simple feedback mechanism
  if (ur > THRESHOLD) {
    voltage --;
    if (voltage < 0) {
      voltage = 0;
    }
  }
  if (ur < THRESHOLD) {
    voltage ++;
    if (voltage > 255) {
      voltage = 255;
    }
  }
  analogWrite (SOURCE_PIN, voltage);

  // Delaying without 'delay' function.
  now = millis ();
  if (now - then > DELAY) {
    Serial.print (ur);
    Serial.print ("\t");
    Serial.print (us);
    Serial.print ("\t");
    Serial.print (uw);
    Serial.print ("\t");
    Serial.print (u);
    Serial.print ("\t");
    Serial.println (voltage);
    then = now;
  }
}
