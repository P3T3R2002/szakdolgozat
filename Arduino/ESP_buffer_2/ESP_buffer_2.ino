#include <Arduino.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

struct SPEED_DATA {
  public:
    int speed;
    int date;
    struct SPEED_DATA* next;
};

struct ANGLE_DATA {
  public:
    int angle;
    struct ANGLE_DATA* next;
};

struct SPEED_DATA* current_speed_ptr;
struct SPEED_DATA* start_speed_ptr;
struct ANGLE_DATA* current_angle_ptr;
struct ANGLE_DATA* start_angle_ptr;
int speed;
int angle;
int buffer = 13600;

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
  server.on("/date", handleDate);

  server.begin();                           //Start server
  Serial.println("HTTP server started");
  
  pinMode(A0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D5, INPUT);

  struct SPEED_DATA* current_speed = (struct SPEED_DATA*)malloc(sizeof(struct SPEED_DATA));
  current_speed->speed = NULL;
  current_speed->date = NULL;
  current_speed->next = NULL;
  current_speed_ptr = current_speed;
  start_speed_ptr = current_speed_ptr;

  struct ANGLE_DATA* current_angle = (struct ANGLE_DATA*)malloc(sizeof(struct ANGLE_DATA));
  current_angle->angle = NULL;
  current_angle->next = NULL;
  current_angle_ptr = current_angle;
  start_angle_ptr = current_angle_ptr;
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    server.handleClient(); 
    get_DATA();
  } else {  
    WiFi.begin(ssid, password);

    Serial.println();
    Serial.print("Connecting...");

    while(WiFi.status() != WL_CONNECTED) {
      get_DATA();
    }
  }
}

void handleRoot() {
  if (start_speed_ptr->next != NULL && millis() % 200 < 190) {

    JsonDocument json;
    json["speed"] = start_speed_ptr->speed;
    json["date"] = start_speed_ptr->date;
    
    struct SPEED_DATA* tmp = start_speed_ptr->next;
    free(start_speed_ptr);
    start_speed_ptr = tmp;
    buffer += 12;

    if (start_angle_ptr->next != NULL && start_speed_ptr->date % 1000 == 0) {

      json["angle"] = start_angle_ptr->angle;

      struct ANGLE_DATA* tmp = start_angle_ptr->next;
      free(start_angle_ptr);
      start_angle_ptr = tmp;
      buffer += 8;
    }

    String output;
    serializeJson(json, output);
    server.send(200, "text/plain", output);

  }
}

void handleDate() {

    JsonDocument json;

    json["date"] = millis();

    String output;
    serializeJson(json, output);
    server.send(200, "text/plain", output);
}

void get_DATA() {
  
  long long int time = millis();
  if (time % 200 == 0 && buffer > 0 && int(millis()/100) != int(current_speed_ptr->date/100)) {
    struct SPEED_DATA* current_speed = (struct SPEED_DATA*)malloc(sizeof(struct SPEED_DATA));
    
    current_speed->speed = analogRead(A0);
    current_speed->date = millis();
    current_speed->next = NULL;

    current_speed_ptr->next = current_speed;
    current_speed_ptr = current_speed_ptr->next;
    buffer -= 12;

    if (time % 1000 == 0 && buffer != 0) {
      struct ANGLE_DATA* current_angle = (struct ANGLE_DATA*)malloc(sizeof(struct ANGLE_DATA));

      current_angle->angle = (1)*digitalRead(D1) + (2)*digitalRead(D2) + (2*2)*digitalRead(D3);
      current_angle->next = NULL;

      current_angle_ptr->next = current_angle;
      current_angle_ptr = current_angle_ptr->next;
      buffer -= 8;
    }
  }
}