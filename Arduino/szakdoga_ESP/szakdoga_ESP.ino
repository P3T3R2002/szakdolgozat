#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

const char* ssid = "Telekom-ecb5b0";
const char* password = "QJZ3KDNLNMM3";
//const char* ssid = "Xiaomi 11T";
//const char* password = "aA123456";
//const char* ssid = "Banyai";
//const char* password = "24637954";

IPAddress staticIP(192, 168, 0, 200); // ESP32 static IP
IPAddress gateway(192, 168, 0, 1);    // IP Address of your network gateway (router)
IPAddress subnet(255, 255, 255, 0);   // Subnet mask



int sensorState = 0;

WiFiClientSecure client;
ESP8266WebServer server(8080);

void setup() {

  Serial.begin(9600);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting...");

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.print("NodeMCU IP Address: ");
  Serial.println(WiFi.localIP());


  server.on("/", handleRoot);  //Associate handler function to path
  server.on("/windspeed", handleSpeed);
  server.on("/windangle", handleAngle);

  server.begin();                           //Start server
  Serial.println("HTTP server started");
  
  pinMode(A0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D5, INPUT);

}

void loop() {
  server.handleClient();  
}

void handleRoot() {
  server.send(200, "text/plain", "I exist as an ESP8266.");
}

void handleSpeed() {
  float sensorState = analogRead(A0);
  float voltage = sensorState*30/1024;
  server.send(200, "text/plain", String(voltage).c_str());
}

void handleAngle() {
  int voltage = 0;
  voltage += (1)*digitalRead(D1);
  voltage += (2)*digitalRead(D2);
  voltage += (2*2)*digitalRead(D3);
  voltage += (2*2*2)*digitalRead(D5);
  char* angle;
  switch (voltage) {
    case 0:  angle = "North";             break;
    case 1:  angle = "North-North-East";  break;
    case 2:  angle = "North-East";        break;
    case 3:  angle = "East-North-East";   break;

    case 4:  angle = "East";              break;
    case 5:  angle = "East-South-East";   break;
    case 6:  angle = "South-East";        break;
    case 7:  angle = "South-South-East";  break;

    case 8:  angle = "South";             break;
    case 9:  angle = "South-South-West";  break;
    case 10: angle = "South-West";        break;
    case 11: angle = "West-South-West";   break;

    case 12: angle = "West";              break;
    case 13: angle = "West-North-West";   break;
    case 14: angle = "North-West";        break;
    case 15: angle = "North-North-West";  break;

    default: angle = "Error: Default.";   break;
  }
  server.send(200, "text/plain", String(angle).c_str());
}