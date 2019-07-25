#include <ESP8266WiFi.h>
#include "RoboClaw.h"

RoboClaw roboclaw = RoboClaw(&Serial, 10000);

const char pw[] = "basicmicro";
const char ap_name[] = "esp8266_test";

WiFiServer server(80);

String header = "HTTP/1.1 200 OK\r\n\Content-Type: text/html\r\n\r\n";
String html1 = "<!DOCTYPE html><html><head><title>Control RoboClaw</title></head><body><h1>Click to operate motor channel one</h1>";
String html2 = "<form id='on' action='motor_on'><input class='button' type='submit' value='motor on'></form>";
String html3 = "<form id='off' action='motor_off'><input class='button' type='submit' value='motor off'></form>";
String html4 = "</body></html>";
String request = "";

void setup() {
  roboclaw.begin(38400);
  Serial.swap();
  boolean conn = WiFi.softAP(ap_name, pw);
  server.begin();
}

void loop() {
    WiFiClient client = server.available();
    if(!client) {return;}

    request = client.readStringUntil('\r');
    if (request.indexOf("motor_on") > 0){roboclaw.ForwardM1(0x80, 127);}
    else if (request.indexOf("motor_off") > 0){roboclaw.ForwardM1(0x80, 0);}

    client.flush();
    client.print(html1);
    client.print(html2);
    client.print(html3);
    client.print(html4);

    delay(5);
}
