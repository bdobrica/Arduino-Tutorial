#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>

#ifndef LOCAL_SSID
#define LOCAL_SSID  "nume-retea-wireless" /** numele rețelei, așa cum apare el în telefon sau pe calculator */
#define LOCAL_PASS  "parola-de-la-wireless" /** parola de la wireless */
#endif

const char* ssid     = LOCAL_SSID;
const char* password = LOCAL_PASS;
const uint16_t all_pixels = 150;


ESP8266WebServer server(80);
Adafruit_NeoPixel pixels(all_pixels, D5, NEO_GRB + NEO_KHZ800);

uint16_t _min = 1023;
uint16_t _max = 0;

uint16_t _length = 32;

uint16_t _hue = 40000;
uint8_t _mode = 1;

uint16_t pixel = 0;
uint16_t last_pixel = 0; 

void handle_root() {
  server.send(200, "text/html", "");
}

void handle_404() {
  server.send(404, "application/json", "{\"error\":1,\"message\":\"not found\"}");
}

void handle_form () {
  long convert; /** variabilă temporară în care convertesc șiruri în numere */
  if (server.method() != HTTP_POST) { /** dacă cererea nu e HTTP POST */
    server.send(405, "application/json", "{\"error\":1,\"message\":\"method not allowed\"}");
    /** trimit către client codul de eroare asociat cu method not allowed, 405 */
    return;
  }
  for (uint8_t c = 0; c < server.args(); c++) { /** scanez toți parametrii */
    if (server.argName(c) == String("mode")) { /** dacă am găsit mode */
      convert = server.arg(c).toInt(); /** în convert pun valoarea parametrului */
      /** dar dacă cumva valoarea e mai mare de 1, pun în _mode 1 */
      _mode = convert < 0 ? 0 : (convert > 255 ? 255 : convert);
    }
    if (server.argName(c) == String("hue")) { /** dacă am găsit hue */
      convert = server.arg(c).toInt(); /** convertesc valoarea lui în număr */
      /** iar dacă numărul e mai mare de 65535, pun în _hue 65535 */
      _hue = convert < 0 ? 0 : (convert > 65535 ? 65535 : convert);
    }
    if (server.argName(c) == String("length")) { /** dacă am găsit length */
      convert = server.arg(c).toInt(); /** convertesc valoarea lui în număr */
      /** dacă numărul e mai mare decât all_pixels - 1, pun în _length all_pixels - 1 */
      _length = convert <  0 ? 0 : (convert > all_pixels - 1 ? all_pixels - 1 : convert);
    }
    /** dacă găsesc reset cu valoarea on, reset valorile pentru _min și _max */
    if (server.argName(c) == String("reset") && server.arg(c) == String("on")) {
      _min = 1023;
      _max = 0;
    }
  }
  /** trimit ca răspuns valorile nou stabilite */
  server.send (200, "application/json",
    "{\"mode\":" + String(_mode) +
    ",\"length\":" + String(_length) +
    ",\"hue\":" + String(_hue) +
    ",\"min\":" + String(_min) +
    ",\"max\":" + String(_max) +
    "}");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }
  
  Serial.println("");
  Serial.println(WiFi.localIP());
  server.on("/", handle_root);
  server.on("/rpc/", handle_form); /** aici asociez handle_form cu serverul web */
  server.onNotFound(handle_404);
  server.begin();
  Serial.println("S");
  
  pixels.begin();
  Serial.println("P");   
  pixels.clear();
  Serial.println("C");

  pinMode (D6, OUTPUT);
  pinMode (D7, OUTPUT);
  pinMode (D8, OUTPUT);
  digitalWrite (D6, LOW);
  digitalWrite (D7, LOW);
  digitalWrite (D8, LOW);
}

void loop() {
  uint8_t
    b_map;
  uint16_t
    a_val,
    a_map;
  uint32_t
    color;
  server.handleClient();
  
  a_val = analogRead(A0);
  if (_min > a_val) {
    _min = a_val;
  }
  if (_max < a_val) {
    _max = a_val;
  }

  switch (_mode) {
    case 0:
      a_map = _max > _min ? (uint16_t) floor (65535.0 * (float)(a_val - _min) / (float) (_max - _min)) : 0; 
      color = pixels.gamma32(pixels.ColorHSV(a_map));
      break;
    case 1:
      b_map = _max > _min ? (uint8_t) floor (255.0 * (float)(a_val - _min) / (float) (_max - _min)) : 0; 
      color = pixels.gamma32(pixels.ColorHSV(_hue, 255, b_map));
      break;
  }
  
  pixels.setPixelColor(pixel, color);
  last_pixel = (all_pixels + pixel - _length) % all_pixels;
  pixel = (pixel + 1) % all_pixels;
  pixels.setPixelColor(last_pixel, pixels.Color(0,0,0));
  pixels.show();
}