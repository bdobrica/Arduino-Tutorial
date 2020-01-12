#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h> /** asta e biblioteca nouă, restul le-am pus pentru context */

#ifndef LOCAL_SSID /** protecție să nu definesc rețeaua de două ori */
#define LOCAL_SSID "nume-retea-wireless" /** numele rețelei, așa cum apare el în telefon sau pe calculator */
#define LOCAL_PASS  "parola-de-la-wireless" /** parola de la wireless */
#endif

const char* ssid     = LOCAL_SSID; /** pun în constante de tip char* valorile definite mai sus */
const char* password = LOCAL_PASS;

ESP8266WebServer server(80); /** aici definesc serverul web, folosind portul 80, adică normal */

void handle_root() { /** functia asta spune ce se intampla cand accesez adresa direct in browser */
  server.send(200, "text/html", "heei, asta e prima pagina!"); /** pur si simplu afisez in browser mesajul */
  /** ce mi se pare interesant, e ca in loc de mesaj poti sa pui codul HTML al oricărei pagini */
  /** primul parametru al server.send, 200, este statusul răspunsului HTML; 200 = OK */
  /** al doilea parametru, "text/html", este tipul raspunsului, în cazul ăsta o pagină web */
  /** al treilea parametru, "heei, asta e prima pagina!", este conținutul răspunsului */
}

void handle_404() { /** functia asta spune ce se intampla cand accesez o pagină care nu există */
  server.send(404, "application/json", "{\"error\":1,\"message\":\"not found\"}");
  /** trimit către browser un mesaj de eroare: */
  /** primul parametru, statusul răspunsului, înseamnă 404 = Page Not Found */
  /** al doilea parametru spune că întorc un obiect de tip json, cu error=1 și message=not found */
  /** al treilea parametru conține obiectul json */
}

void setup() { /** pus aici pentru context */
  Serial.begin(115200); /** initializez conexiunea serială, să văd ce se întâmplă, viteza 115200 bauds */
  WiFi.begin(ssid, password); /** încerc să mă conectez la wireless */
  Serial.println(""); /** trimit o linie nouă către terminalul serial */
  
  while (WiFi.status() != WL_CONNECTED) { /** atât timp cât încă nu m-am conectat la wireless */
    delay(500); /** aștept jumătate de secundă */
    Serial.print("#"); /** și trimit un # către terminalul serial, după care mai încerc o dată */
  }
  
  Serial.println("");
  Serial.println(WiFi.localIP());
  /** urmează bucata nouă: */
  server.on("/", handle_root); /** conectez funcția pentru index la serverul web */
  server.onNotFound(handle_404); /** conectez funcția pentru pagini care nu există la serverul web */
  server.begin(); /** pornesc serverul web */
  Serial.println("S"); /** trimit S către terminal ca să știu că a pornit și serverul web */
}

void loop() { /** pus aici pentru context, urmează instrucțiunile noi */
  server.handleClient(); /** asteapta conexiuni de la browsere și dacă sunt, interpretează-le */
  /** ca limitare, serverul web poate interpreta câte o conexiune pe rând */
}