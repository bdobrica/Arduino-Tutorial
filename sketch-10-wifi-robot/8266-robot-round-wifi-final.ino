#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef LOCAL_SSID
#define LOCAL_SSID "nume-retea-wireless"
#define LOCAL_PASS  "parola-de-la-wireless"
#endif

#ifndef MOTOR_PINS
#define MOTOR_PINS
#define MOTOR_A_IN1 D1
#define MOTOR_A_IN2 D2
#define MOTOR_B_IN1 D3
#define MOTOR_B_IN2 D4
#endif

const char* ssid     = LOCAL_SSID;
const char* password = LOCAL_PASS;

uint8_t motor_A = 0x80;
uint8_t motor_B = 0x80;

bool motor_change = false;

ESP8266WebServer server(80);

void handle_root() {
  server.send(200, "text/html", "<!DOCTYPE html>\n<html lang=\"en\"><head><meta charset=\"utf-8\"><title>Robot</title></head><body><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js\"></script><script src=\"https://rawgit.com/ngryman/jquery.finger/v0.1.2/dist/jquery.finger.js\"></script><script>var motor={u:'/rpc/',a:128,b:128,busy:0,body:$('body'),move:function(){if(this.busy)return;this.body.css({'backgroundColor':'blue'});this.busy=1;$.ajax({type:'POST',url:this.u,data:{'motor_a':this.a,'motor_b':this.b},context:this,success:function(r){this.body.css({'backgroundColor':'transparent'});this.busy=0;},error:function(){this.body.css({'backgroundColor':'transparent'});this.busy=0;}});},stop:function(){this.a=this.b=128;this.move()}},w=$(window);w.on('drag',motor,function(ev){var x=2*ev.dx/w.width(),y=-2*ev.dy/w.height(),t=Math.atan2(y,x),a=Math.sign(y)*Math.sqrt(x*x+y*y)*(1-Math.cos(t)),b=Math.sign(y)*Math.sqrt(x*x+y*y)*(1+Math.cos(t));motor.a=Math.floor(128+127*(a>1?1:(a<-1?-1:a)));motor.b=Math.floor(128+127*(b>1?1:(b<-1?-1:b)));motor.move()}).on('doubletap',motor,function(ev){ev.data.stop()});</script></body></html>");
}

void handle_404() {
  server.send(404, "application/json", "{\"error\":1,\"message\":\"not found\"}");
}

void run_motor (uint8_t motor, uint8_t in1, uint8_t in2) {
  if ((motor & 0x80) == 0x80) { // forward
    analogWrite (in1, (0x7f & motor) << 3);
    //digitalWrite (in1, HIGH);
    digitalWrite (in2, LOW);
  }
  else { // backwards
    digitalWrite (in1, LOW);
    analogWrite (in2, (0x7f - (0x7f & motor)) << 3);
    //digitalWrite (in2, HIGH);
  }
}

void set_motor (uint8_t &motor, String value) {
  long conversion = 0;
  motor_change = true;
  
  if (value == String ("up")) {
    conversion = motor + 0x08;
    if (conversion > 0xFF) {
      conversion = 0xFF;
    }
    motor = conversion;
    return;
  }
  if (value == String ("dw")) {
    conversion = motor - 0x08;
    if (conversion < 0x00) {
      conversion = 0x00;
    }
    motor = conversion;
    return;
  }
  conversion = value.toInt();
  if (conversion < 1 || conversion > 0xFF) {
    conversion = 0x80;
  }
  motor = conversion;
}

void handle_form () {
  if (server.method() != HTTP_POST) {
    server.send(405, "application/json", "{\"error\":1,\"message\":\"method not allowed\"}");
    return;
  }
  for (uint8_t c = 0; c < server.args(); c++) {
    if (server.argName(c) == String("motor_a")) {
      set_motor(motor_A, server.arg(c));
    }
    if (server.argName(c) == String("motor_b")) {
      set_motor(motor_B, server.arg(c));
    }
  }
  server.send (200, "application/json", "{\"motor_A\":" + String(motor_A) + ",\"motor_B\":" + String(motor_B) + "}");
  Serial.print("A");
  Serial.println(motor_A);
  Serial.print("B");
  Serial.println(motor_B);
}

void setup() {
  pinMode (MOTOR_A_IN1, OUTPUT);
  pinMode (MOTOR_A_IN2, OUTPUT);
  pinMode (MOTOR_B_IN1, OUTPUT);
  pinMode (MOTOR_B_IN2, OUTPUT);
  
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
  server.on("/rpc/", handle_form);
  server.onNotFound(handle_404);
  server.begin();
  Serial.println("S");
}

void loop() {
  server.handleClient();
  if (!motor_change) {
    return;
  }
  motor_change = false;
  run_motor (motor_A, MOTOR_A_IN1, MOTOR_A_IN2);
  run_motor (motor_B, MOTOR_B_IN1, MOTOR_B_IN2);
}
