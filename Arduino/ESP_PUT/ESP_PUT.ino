#include <Arduino.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

struct DATA {
  public:
    int speed;
    int angle;
    int date;
    struct DATA* next;
};

struct DATA* current_ptr;
struct DATA* start_ptr;
int buffer = 1000;

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

  Serial.begin(115200);
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


  server.on("/", handleRoot);

  server.begin();                           //Start server
  Serial.println("HTTP server started");
  
  pinMode(A0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D5, INPUT);

  struct DATA* current = (struct DATA*)malloc(sizeof(struct DATA));
  current->speed = NULL;
  current->angle = NULL;
  current->date = NULL;
  current->next = NULL;
  current_ptr = current;
  start_ptr = current_ptr;
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    server.handleClient(); 
    getDATA();
  } else {  
    WiFi.begin(ssid, password);

    Serial.println();
    Serial.print("Connecting...");

    while(WiFi.status() != WL_CONNECTED) {
      getDATA();
    }
  }
}

void handleRoot() {
  if (start_ptr->next != NULL && millis() % 200 < 190) {
    
    JsonDocument json;
    json["speed"] = start_ptr->speed;
    json["angle"] = start_ptr->angle;
    json["date"] = start_ptr->date;

    String output;
    serializeJson(json, output);
    server.send(200, "text/plain", output);

    struct DATA* tmp = start_ptr->next;
    free(start_ptr);
    start_ptr = tmp;
    buffer++;
    
    Serial.println(buffer);
  }
}

void getDATA() {
  
  long long int time = millis();
  if (time % 200 == 0 && buffer != 0 && int(millis()/100) != int(current_ptr->date/100)) {
    struct DATA* current = (struct DATA*)malloc(sizeof(struct DATA));
    int angle = 0;
    angle += (1)*digitalRead(D1);
    angle += (2)*digitalRead(D2);
    angle += (2*2)*digitalRead(D3);
    angle += (2*2*2)*digitalRead(D5);
    current->angle = angle;
    current->speed = analogRead(A0);
    current->date = millis();
    current->next = NULL;
    current_ptr->next = current;
    current_ptr = current_ptr->next;
    buffer--;
  }
}