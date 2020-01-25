#include <ESP8266WiFi.h> /** biblioteca prin care ESP8266 acceseaza conexiunea wi-fi */
#include <WiFiClient.h> /** biblioteca prin care ESP8266 folosește wi-fi și își ia adresa de IP */
#include <ESP8266WebServer.h> /** biblioteca prin care ESP8266 pornește serverul web, nimic nou */
#include <Adafruit_NeoPixel.h> /** dar asta da, se ocupă de ledurile NeoPixel */

#ifndef LOCAL_SSID /** protecție să nu definesc rețeaua de două ori */
#define LOCAL_SSID  "nume-retea-wireless" /** numele rețelei, așa cum apare el în telefon sau pe calculator */
#define LOCAL_PASS  "parola-de-la-wireless" /** parola de la wireless */
#endif

const char* ssid     = LOCAL_SSID; /** pun în constante de tip char* valorile definite mai sus */
const char* password = LOCAL_PASS;
const uint16_t all_pixels = 150; /** aici e numărul de leduri din banda pe care o am conectată */

ESP8266WebServer server(80); /** aici definesc serverul web, folosind portul 80, adică normal */
Adafruit_NeoPixel pixels(all_pixels, D5, NEO_GRB + NEO_KHZ800); /** aici definesc banda cu leduri */
/**
 * parametrii sunt:
 * all_pixels, constantă de tip unsigned int, numărul de leduri din bandă
 * D5, este terminalul esp8266 witty la care e conectată banda cu leduri
 * NEO_GRB + NEO_KHZ800, sunt două constante care activează tipul dispunerii ledurilor în neopixel,
 *     și poate fi NEO_GRB sau NEO_RGB, în timp ce a doua constantă îmi spune viteza de comunicare
 *     NEO_KHZ800 pentru WS2812 sau NEO_KHZ400 pentru WS2811
 */

uint16_t _min = 1023; /** minimum înregistrat de convertorul ADC, 1023 petru că va scădea cu fiecare sunet */
uint16_t _max = 0; /** maximum înregistrată de convertorul ADC, 0 pentru că va crește cu fiecare sunet */
uint16_t _length = 32; /** pentru că sursa nu are suficient curent, voi aprinde doar 32 de leduri deodată */
uint16_t _hue = 40000; /** _hue poate fi între 0 și 65535 și reprezintă culoarea în modul de funcționare 1 */
uint8_t _mode = 1; /** reprezintă modul curent de funcționare */
uint16_t pixel = 0; /** o variabilă care indică pixelul curent */
uint16_t last_pixel = 0; /** o variabilă care indică precedentul pixel */

void handle_root() { /** functia asta spune ce se intampla cand accesez adresa direct in browser */
  /** adică nimic, doar îi spun clientului că nu sunt erori */
  server.send(200, "text/html", "");
}

void handle_404() { /** functia asta spune ce se intampla cand accesez o pagină care nu există */
  /** doar trimit către client un mesaj de eroare */
  server.send(404, "application/json", "{\"error\":1,\"message\":\"not found\"}");
}

void setup() { /** functia de setup, specifica Arduino, ruleaza după reset sau la pornire */
  Serial.begin(115200); /** initializez conexiunea serială, să văd ce se întâmplă, viteza 115200 bauds */
  WiFi.begin(ssid, password); /** încerc să mă conectez la wireless */
  Serial.println(""); /** trimit o linie nouă către terminalul serial */
  
  while (WiFi.status() != WL_CONNECTED) { /** atât timp cât încă nu m-am conectat la wireless */
    delay(500); /** aștept jumătate de secundă */
    Serial.print("#"); /** și trimit un # către terminalul serial, după care mai încerc o dată */
  }
  
  Serial.println(""); /** aici înseamnă că m-am conectat, așa că trec la linia următoare în terminal */
  Serial.println(WiFi.localIP()); /** și trimit adresa de IP ca să știu unde mă conectez cu calculatorul */
  server.on("/", handle_root); /** conectez funcția pentru index la serverul web */
  server.onNotFound(handle_404); /** conectez funcția pentru pagini care nu există la serverul web */
  server.begin(); /** pornesc serverul web */
  Serial.println("S"); /** trimit S către terminal ca să știu că a pornit și serverul web */
  
  /** până aici, codul e foarte asemănător cu cel de la robotul wi-fi */

  pixels.begin(); /** inițializez banda cu leduri */
  Serial.println("P"); /** trimit P către termina să știu că banda e inițializată */
  pixels.clear(); /** sting toți pixelii din bandă */
  Serial.println("C"); /** și trimit C prin serial să știu că am făcut asta */

  pinMode (D6, OUTPUT); /** stabilesc terminalele D6, D7, D8 ca ieșiri */
  pinMode (D7, OUTPUT); /** la terminalele astea sunt conectate leduri direct pe placuta witty */
  pinMode (D8, OUTPUT); /** asa ca am grija sa le sting */
  digitalWrite (D6, LOW); /** sting ledul conectat la D6 */
  digitalWrite (D7, LOW); /** sting ledul conectat la D7 */
  digitalWrite (D8, LOW); /** sting ledul conectat la D8 */
}

void loop() { /** functia asta se repetă la nesfârșit, după setup */
  /** am nevoie de câteva variabile locale */
  uint8_t
    b_map; /** variabilă pentru intensitatea pixelului când _mode = 1 */
  uint16_t
    a_val, /** variabilă pentru valoarea măsurată de ADC */
    a_map; /** variabilă pentru culoarea pixelului când _mode = 0 */
  uint32_t
    color; /** variabilă pentru culoarea pixelului, în format NeoPixel */
  server.handleClient(); /** aici văd dacă serverul web a primit ceva date */
  
  a_val = analogRead(A0); /** citesc în a_val valoarea din ADC; aici e nivelul sunetului */
  if (_min > a_val) { /** dacă nivelul sunetului e mai mic decât cel minim */
    _min = a_val; /** redefinesc valoarea minimă */
  }
  if (_max < a_val) { /** dacă nivelul sunetului e mai mare decât cel maxim */
    _max = a_val; /** redefinesc valoarea maximă */
  }

  switch (_mode) { /** în funcție de modul de operare */
    case 0: /** dacă modul e 0, atunci: */
      /**
       * găsesc punctul a_map în intervalul [0,65535] care împarte intervalul
       * în aceleași proporții în care a_val împarte intervalul [_min, _max]
       * în felul ăsta, culoarea pixelului e proporțională cu intensitatea sunetului
       */
      a_map = _max > _min ? (uint16_t) floor (65535.0 * (float)(a_val - _min) / (float) (_max - _min)) : 0; 
      color = pixels.gamma32(pixels.ColorHSV(a_map)); /** transform numărul în culoare */
      break;
    case 1:
      /**
       * găsesc punctul b_map în intervalul [0,255] care împarte intervalul
       * în aceleași proporții în care a_val împarte intervalul [_min, _max]
       * în felul ăsta, intensitatea luminoasă a pixelului e proporțională cu sunetul
       */
      b_map = _max > _min ? (uint8_t) floor (255.0 * (float)(a_val - _min) / (float) (_max - _min)) : 0;
      /** pornind de la culoarea _hue, stabilesc culoarea pixelului având intensitatea b_map */
      color = pixels.gamma32(pixels.ColorHSV(_hue, 255, b_map));
      break;
  }
  
  pixels.setPixelColor(pixel, color); /** fac pixelul curent pixel de culoarea color */
  /**
   * consider că toți pixelii sunt așezați în cerc. pe cerc, calculez pixelul pe care
   * trebuie să-l șterg -> păstez ultimii _length pixel și îl șterg pe _length+1
   */
  last_pixel = (all_pixels + pixel - _length) % all_pixels;
  pixel = (pixel + 1) % all_pixels; /** trec la pixelul următor, în cerc */
  pixels.setPixelColor(last_pixel, pixels.Color(0,0,0)); /** sting ultimul pixel */
  pixels.show(); /** trimit datele către banda cu leduri */
}