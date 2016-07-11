/* The following sketch transforms a simple LED in light sensor.
 * It senses the light for a small amount of time, after which it
 * turns the led on or off if light has been detected or not.
 * 
 * We're connecting:
 * PIN 12 ---|>|--- PIN 11
 */
#define SENSOR_ANODE		11
#define SENSOR_CATHODE		12
/* The principle of operation is simple: polarized in reverse,
 * the led behaves as a small capacitor (pF). Removing the power,
 * puts the led in sensing mode. Each photon that hits the junction
 * helps discharge the capacitor. We only need to wait a certain
 * amount of time and compare the capacitor's charge with a known
 * reference. If it's lower, than light has been sensed.
 *
 * So, here we're setting up the charge time (in microseconds) and
 * the sensibility (we're calling it ISO, as in cameras) also as after
 * time interval (in microseconds).
 */
#define CHARGE_DELAY		50
#define ISO					80
/* Next, we're setting how long the led will be turned on after it
 * sensed light. This value is in miliseconds.
 */
#define LIGHT_DELAY			500

/* We're setting here the only pin that doesn't change its mode of
 * operation. The cathode of the LED. As a sensor, it's connected to
 * GND (0V) and in normal operation at Vcc (5V)
 */
void setup () {
	pinMode (SENSOR_CATHODE, OUTPUT);
	}

void loop () {
	/* We're charging the led capacitor by setting the value for the
	 * catode to 0V and the value for anode to Vcc.
	 */
	digitalWrite (SENSOR_CATHODE, LOW);
	pinMode (SENSOR_ANODE, OUTPUT);
	digitalWrite (SENSOR_ANODE, HIGH);
	/* The charge is hold for CHARGE_DELAY microseconds. The function
	 * delayMicroseconds is the same as delay, the only difference
	 * being the units used to measure time.
	 */
	delayMicroseconds (CHARGE_DELAY);

	/* Next, we're setting the anode pin for measurement. When set as
	 * INPUT, the pin is in a high impedance state.
	 */
	pinMode (SENSOR_ANODE, INPUT);
	/* Before measuring the value on the anode pin, we wait ISO
	 * microseconds to capture photons.
	 */
	delayMicroseconds (ISO);
	
	/* digitalRead takes a single argument, the pin for which the value
	 * needs to be measured. It can output either LOW (for logic 0), or
	 * HIGH (for logic 1). Here we check if the capacitor has discharged,
	 * so if the value is LOW.
	 */
	if (digitalRead (SENSOR_ANODE) == LOW) {
		/* If the capacitor has discharged, we're directly powering the
		 * led, by setting the cathode pin HIGH and the anode pin LOW.
		 */
		digitalWrite (SENSOR_CATHODE, HIGH);
		pinMode (SENSOR_ANODE, OUTPUT);
		digitalWrite (SENSOR_ANODE, LOW);
		/* Keep the led on for LIGHT_DELAY miliseconds.
		*/
		delay (LIGHT_DELAY);
		}
	}