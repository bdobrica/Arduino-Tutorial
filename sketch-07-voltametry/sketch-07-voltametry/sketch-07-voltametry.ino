#define CAP_A A0
#define CAP_K A1

#define WINDOW_MS 2000
#define MAX_SAMPLES 10

bool charge = true;
bool dir = true;
unsigned long int then, now;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
}

void loop() {
  int c, v;

  if (charge) {
    pinMode (CAP_A, OUTPUT);
    pinMode (CAP_K, OUTPUT);

    if (dir) {
      digitalWrite (CAP_A, LOW);
      digitalWrite (CAP_K, HIGH);      
    }
    else {
      digitalWrite (CAP_A, HIGH);
      digitalWrite (CAP_K, LOW);
    }

  delay (100);
  Serial.println ("0\t0");
  charge = false;
  pinMode (dir ? CAP_K : CAP_A, INPUT);
  then = millis ();
  return;
  }

  now = millis ();
  if (now - then > WINDOW_MS) {
    dir = !dir;
    charge = true;
    return;
  }

  v = 0;
  for (c = 0; c < MAX_SAMPLES; c++) {
    v += analogRead (dir ? CAP_K : CAP_A);
  }
  v = dir ? (v / MAX_SAMPLES) : (-v / MAX_SAMPLES);

  Serial.print (now - then);
  Serial.print ("\t");
  Serial.println (v);
}
