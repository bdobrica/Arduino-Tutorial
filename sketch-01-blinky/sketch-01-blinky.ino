/* We're defining here a few constants. Constants cannot be change
 * after loading the program on the Arduino. To define a constant,
 * use the primitive (special command) #define followed by the name
 * of the constant and it's value. Everything is separated by white
 * spaces (spaces, tabs). You can use as many whitespaces as you
 * like. The name of the constant must start with a letter and can
 * have only letters, numbers or underscores.
 * 
 * We're connecting:
 * LED1:
 * PIN 12 ---|>|--- GND
 * LED2:
 * PIN 13 ---|>|--- GND
 */
#define LED1_PIN			12
#define LED2_PIN			13
#define LED1_ON_TIME_MS		500
#define LED2_ON_TIME_MS		800

/* The setup function is used to set things up when Arduino boots up.
 * The boot happens after reset or after powering up. What you put
 * here will be run only once, at the beginning. This is the place to
 * initialize peripherals.
 */
void setup () {
	/* pinMode function takes two parameters. The first is the pin for
	 * which is sets the operating mode to. The second is how the pin
	 * behaves. It can be either INPUT (and you can read the value of
	 * the pin with digitalRead function), INPUT_PULLUP (same as INPUT,
	 * but also connects a resistor from the pin to Vcc, meaning the
	 * default pin value will be HIGH), OUTPUT (and you can set the
	 * value of the pin, with digitalWrite to either LOW or HIGH).
	 *
	 * Here, we're using the function to let two pins drive two leds.
	 */
	pinMode (LED1_PIN, OUTPUT);
	pinMode (LED2_PIN, OUTPUT);
	
	/* digitalWrite function takes two paramenters. The first is the pin
	 * for which it will set the value to. The second is what voltage to
	 * output at the selected pin. If it is LOW, the pin is connected to
	 * ground (0V). If it is HIGH, the pin is connected to Vcc (usually
	 * 5V). Arduino pins can source or sink only 40mA, so be careful.
	 * Also, OUTPUT pins can be used to output PWM signals (pulse width
	 * modulated signals), but only for selected pins (marked with ~ on
	 * the Arduino board). Use analogWrite function for this.
	 * 
	 * Here, we're using the function to turn of both leds, as both their
	 * anode and cathode are connected to 0V.
	 */
	digitalWrite (LED1_PIN, LOW);
	digitalWrite (LED2_PIN, LOW);
	}

/* The loop function is used to run your code an infinite number of times.
 * Actually, it will run until powering off or reseting the Arduino.
 * This is usefull, and a little different from a normal computer program,
 * as the Arduino is intended to be programmed once to perform a specific
 * duty. In this case, turning two leds on and off alternatively.
 */
void loop () {
	/* First, we're turning the first led on, and the second led off.
	 * The order is usually important, but since the commands are executed
	 * fairly quicly (under 2uS), we will not notice the delay.
	 */
	digitalWrite (LED1_PIN, HIGH);
	digitalWrite (LED2_PIN, LOW);
	/* delay is one of the most used functions in Arduino. It allows the
	 * Arduino to take a break from current operations for the specified
	 * ammount of ms. The only parameter is the number of ms to halt the
	 * Arduino operations.
	 * 
	 * Here, since we've turned the first led on and the second one off,
	 * we're holding the state for a specified number of ms.
	 */
	delay (LED1_ON_TIME_MS);
	/* We then switch the first led off and the second led on.
	 */
	digitalWrite (LED1_PIN, LOW);
	digitalWrite (LED2_PIN, HIGH);
	/* And we wait in this state for a specified amount of time.
	 */
	delay (LED2_ON_TIME_MS);
	/* Now, the Arduino will go to the begining of the loop function, and
	 * will execute the above instructions again, and again, and again ...
	 */
	}
