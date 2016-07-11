#define PORT_SPEED			9600
#define TICK_DELAY_MS		1000

void setup () {
	Serial.begin (PORT_SPEED);
	}

void loop () {
	Serial.println ("Tick!");
	delay (TICK_DELAY_MS);
	}