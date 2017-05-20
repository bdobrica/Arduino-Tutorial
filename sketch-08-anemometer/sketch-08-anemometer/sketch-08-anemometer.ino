#define SENSOR_PIN A0

/*
 * The equation governing the wire behaviour is:
 * I^2 R0 (1 + C (Tw - Ta)) = (A + B v^n) (Tw - Ta),
 * where:
 *  I = the current passing the wire
 *  R0 = the wire initial resistance
 *  C = wire thermal coeff
 *  Tw = wire temperature
 *  Ta = ambient temperature
 *  A, B, n = heat transfer constants, depend on material and
 *            wire geometry
 *
 * Connection:
 *
 * (5V) <---[R]---+---[B]---| GND
 *				  |
 *			  SENSOR_PIN
 *
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  analogReference (INTERNAL);
}

void loop() {
  // put your main code here, to run repeatedly:
  int u = analogRead (SENSOR_PIN);
  Serial.println (u);
  delay (1000);
}
