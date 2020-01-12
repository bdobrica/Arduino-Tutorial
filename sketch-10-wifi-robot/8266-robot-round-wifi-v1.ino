#include <ESP8266WiFi.h> /** biblioteca prin care ESP8266 acceseaza conexiunea wi-fi */
#include <WiFiClient.h> /** biblioteca prin care ESP8266 folosește wi-fi și își ia adresa de IP */

#ifndef LOCAL_SSID /** protecție să nu definesc rețeaua de două ori */
#define LOCAL_SSID "nume-retea-wireless" /** numele rețelei, așa cum apare el în telefon sau pe calculator */
#define LOCAL_PASS  "parola-de-la-wireless" /** parola de la wireless */
#endif

const char* ssid     = LOCAL_SSID; /** pun în constante de tip char* valorile definite mai sus */
const char* password = LOCAL_PASS;

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
}

void loop() { /** deocamdată nu pun nimic în loop, vreau doar să văd că se conectează */
}