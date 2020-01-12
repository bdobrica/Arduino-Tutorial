#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h> /** asta e biblioteca nouă, restul le-am pus pentru context */

#ifndef LOCAL_SSID /** pus aici pentru context */
#define LOCAL_SSID "nume-retea-wireless"
#define LOCAL_PASS  "parola-de-la-wireless"
#endif
/** urmează definițiile noi: */
#ifndef MOTOR_PINS /** protecție, ca să nu definesc terminalele de două ori */
#define MOTOR_PINS /** definit doar ca să verific dacă nu l-am definit din nou */
#define MOTOR_A_IN1 D1 /** IN1 al punții H e conectat la D1 al ESP8266 */
#define MOTOR_A_IN2 D2 /** IN2 al punții H e conectat la D2 al ESP8266 */
#define MOTOR_B_IN1 D3 /** IN3 al punții H e conectat la D3 al ESP8266 */
#define MOTOR_B_IN2 D4 /** IN4 al punții H e conectat la D4 al ESP8266 */
#endif

const char* ssid     = LOCAL_SSID; /** pus aici pentru context */
const char* password = LOCAL_PASS;
/** urmează variabilele noi: */
uint8_t motor_A = 0x80; /** viteza motorului A, înainte cu viteză 0 */
uint8_t motor_B = 0x80; /** viteza motorului B, înainte cu viteză 0 */

bool motor_change = false; /** dacă viteza motorului s-a modificat */

ESP8266WebServer server(80); /** aici definesc serverul web, folosind portul 80, adică normal */

void handle_root() { /** functia asta spune ce se intampla cand accesez adresa direct in browser */
  server.send(200, "text/html", "heei, asta e prima pagina!"); /** pur si simplu afisez in browser mesajul */
  /** ce mi se pare interesant, e ca in loc de mesaj poti sa pui codul HTML al oricărei pagini */
  /** primul parametru al server.send, 200, este statusul răspunsului HTML; 200 = OK */
  /** al doilea parametru, "text/html", este tipul raspunsului, în cazul ăsta o pagină web */
  /** al treilea parametru, "heei, asta e prima pagina!", este conținutul răspunsului */
}

void handle_404() { /** pus aici pentru context */
  server.send(404, "application/json", "{\"error\":1,\"message\":\"not found\"}");
}
/** urmează definiția funcției pentru controlul motorului */
/** parametrii funcției sunt:
 * motor - întreg pe 8 biți, între 0 și 255 = reprezintă viteza motorului
 * in1 - întreg pe 8 biți = terminalul ESP8266 unde e conectat IN1 (IN3, dacă motorul = B)
 * in2 - întreg pe 8 biți = terminalul ESP8266 unde e conectat IN2 (IN4, dacă motorul = B)
 */
void run_motor (uint8_t motor, uint8_t in1, uint8_t in2) {
  /** cu motor & 0x80 extrag doar primul bit; iar rezultatul va fi 0x80, dacă e 1 sau 0x00 dacă e 0 */ 
  if ((motor & 0x80) == 0x80) { /** deci, dacă primul bit e 1, motorul merge înainte */
    /** din foaia de catalog a MX1515, ca motorul să meargă înainte, */
    /** prima intrare trebuie să fie PWM, iar a două LOW */
    /** 0x7f & motor extrage viteza, iar rezultatul va fi între 0x00 și 0x7f */
    /** cum asta înseamnă un număr între 0 și 127, trebuie să-l înmulțesc cu 8 ca să obțin între 0 și 1023 */
    analogWrite (in1, (0x7f & motor) << 3); /** pe in1 pun semnal PWM */
    digitalWrite (in2, LOW); /** pe in2 scriu LOW */
  }
  else { /** dacă nu merge înainte, motorul merge invers */
    /** rationamentul e asemanator, doar că se inversează in1 și in2 */
    /** altă optimizare e să inversez viteza: când rezultatul 0x7f & motor = 0x7f, viteza să fie 0 */
    /** asta deoarece de la 0 la 127 viteza va scădea */
    /** apoi de la 128 la 255, motorul își schimbă sensul și accelerează */
    digitalWrite (in1, LOW); /** pe in1 de data asta scriu LOW */
    analogWrite (in2, (0x7f - (0x7f & motor)) << 3); /** iar pe in2 pun semnal PWM, cu viteza inversata */
  }
}
/** funcția pentru stabilirea vitezei unuia dintre motoare */
/** parametrii funcției sunt:
 * motor - referință către variabila corespunzătoare vitezei motorului
 *       - referință înseamnă că variabila va fi modificată de funcție,
 *       - și noua valoare va fi accesibilă în exteriorul funcției
 * value - String, reprezintă valoarea primită prin intermediul conexiunii wi-fi
 *       - value poate fi dw - și atunci viteza scade cu 8 unități din 255
 *       - value poate fi up - și atunci viteza crește cu 8 unități din 255
 *       - value poate fi un număr între 0 și 255
 */ 
void set_motor (uint8_t &motor, String value) {
  long conversion = 0; /** String::toInt() intoarce o valoare de tip long, eu am nevoie de uint8_t */
  motor_change = true; /** daca am apelat funcția asta, automat viteza motorului s-a modificat */
  
  if (value == String ("up")) { /** dacă valoarea e "up" */
    conversion = motor + 0x08; /** cresc viteza motorului cu 8 */
    if (conversion > 0xFF) { /** dacă ce obțin e mai mare de 255 */
      conversion = 0xFF; /** fac viteza 255 */
      /** aici am folosit o smecherie: dacă conversion era uint8_t, atunci când adunam */
      /** și treceam peste 255 se întorcea din nou de la 0 și n-aș fi știu ce să fac */
      /** dar cum conversion e de tip long, pot să depășesc 255 fără probleme */
    }
    motor = conversion; /** știu sigur că conversion e cel mult 255, așa că stabilesc viteza */
    return; /** nu merg mai departe */
  }
  if (value == String ("dw")) { /** dacă valoarea e "dw" */
    conversion = motor - 0x08; /** scad viteza motorului cu 8 */
    if (conversion < 0x00) { /** dacă ce obțin e mai mic ca zero */
      conversion = 0x00; /** fac viteza 0 */
      /** din nou șmecheria: conversion e long și poate să fie și negativ */
    }
    motor = conversion; /** știu sigur că conversion e cel puțin 0, așa că stabilesc viteza */
    return; /** nu merg mai departe */
  }
  conversion = value.toInt(); /** convertesc șirul de caractere la întreg */
  /** dacă conversia eșuează, conversion = 0 */
  if (conversion < 1 || conversion > 0xFF) { /** dacă conversion e mai mic de 1 sau mai mare de 255 */
    conversion = 0x80; /** opresc motorul */
  }
  motor = conversion; /** altfel, motorul va avea viteza stabilită */
}

void setup() { /** pus aici pentru context */
  /** urmează inițializarea conexiunii cu puntea H */
  pinMode (MOTOR_A_IN1, OUTPUT); /** stabilesc terminalul conectat la IN1 ca ieșire */
  pinMode (MOTOR_A_IN2, OUTPUT); /** stabilesc terminalul conectat la IN2 ca ieșire */
  pinMode (MOTOR_B_IN1, OUTPUT); /** stabilesc terminalul conectat la IN3 ca ieșire */
  pinMode (MOTOR_B_IN2, OUTPUT); /** stabilesc terminalul conectat la IN4 ca ieșire */
  
  Serial.begin(115200); /** pus aici pentru context */
  WiFi.begin(ssid, password);
  Serial.println("");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }
  
  Serial.println("");
  Serial.println(WiFi.localIP());
  server.on("/", handle_root);
  server.onNotFound(handle_404);
  server.begin();
  Serial.println("S");
}

void loop() { /** pus aici pentru context */
  server.handleClient();
  /** urmează bucata nouă de cod care se ocupă de controlul motoarelor */
  if (!motor_change) { /** dacă motor_change e false */
    return; /** întoarce-te la început */
  }
  motor_change = false; /** aici motor_change e true, așa că îl resetez */
  run_motor (motor_A, MOTOR_A_IN1, MOTOR_A_IN2); /** stabilesc viteza pentru motorul A */
  run_motor (motor_B, MOTOR_B_IN1, MOTOR_B_IN2); /** stabilesc viteza pentru motorul B */
}