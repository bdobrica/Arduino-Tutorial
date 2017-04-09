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
#define LED1_ON_TIME_MS		1000
#define LED2_ON_TIME_MS		2000

void setup () {
	pinMode (LED1_PIN, OUTPUT);
	pinMode (LED2_PIN, OUTPUT);
	digitalWrite (LED1_PIN, LOW);
	digitalWrite (LED2_PIN, LOW);
	}
void loop () {
	digitalWrite (LED1_PIN, HIGH);
	digitalWrite (LED2_PIN, LOW);
	delay (LED1_ON_TIME_MS);
	digitalWrite (LED1_PIN, LOW);
	digitalWrite (LED2_PIN, HIGH);
	delay (LED2_ON_TIME_MS);
	}
