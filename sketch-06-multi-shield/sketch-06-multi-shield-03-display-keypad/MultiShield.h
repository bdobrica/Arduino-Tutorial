// MultiShield, v.1.1

#define MS_LATCH_GPIO  4
#define MS_CLOCK_GPIO  7
#define MS_DATA_GPIO   8

#define MS_DEBOUNCE    100

const byte MS_NUMBERS[] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90,
                            0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10,
                            0xBF };
const byte MS_DIGITS[] = { 0xF1, 0xF2, 0xF4, 0xF8 };
const byte MS_KEYS[] = { A1, A2, A3 };

void ms_setup () {
  pinMode (MS_LATCH_GPIO, OUTPUT);
  pinMode (MS_CLOCK_GPIO, OUTPUT);
  pinMode (MS_DATA_GPIO, OUTPUT);

  pinMode (MS_KEYS[0], INPUT);
  pinMode (MS_KEYS[1], INPUT);
  pinMode (MS_KEYS[2], INPUT);
}

void ms_display (byte where, byte what) {
  digitalWrite (MS_LATCH_GPIO, LOW);
  shiftOut (MS_DATA_GPIO, MS_CLOCK_GPIO, MSBFIRST, MS_NUMBERS[what]);
  shiftOut (MS_DATA_GPIO, MS_CLOCK_GPIO, MSBFIRST, MS_DIGITS[where]);
  digitalWrite (MS_LATCH_GPIO, HIGH);
}

void ms_number (unsigned int number) {
  ms_display (0, number / 1000);
  ms_display (1, (number / 100) % 10);
  ms_display (2, (number / 10) % 10);
  ms_display (3, number % 10);
}

bool ms_keypress (byte key) {
  int debounce;
  if (digitalRead (MS_KEYS[key]) == LOW) {
    for (debounce = 0; debounce < MS_DEBOUNCE; debounce ++);
    if (digitalRead (MS_KEYS[key]) == LOW)
     return true;
    }
  return false;
}
