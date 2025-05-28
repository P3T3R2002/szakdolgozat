#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

struct SPEED_DATA {
  uint16_t speed;
  int date;
  struct SPEED_DATA* next;
};

struct ANGLE_DATA {
  uint16_t angle;
  struct ANGLE_DATA* next;
};

struct SPEED_DATA* current_speed_ptr;
struct SPEED_DATA* start_speed_ptr;
struct ANGLE_DATA* current_angle_ptr;
struct ANGLE_DATA* start_angle_ptr;

int speed;
int angle;
int buffer = 201600;


const char* ssid = "Vodafone-B042";
const char* password = "bQLJggB8gFPa63Fd";

//const char* ssid = "Telekom-ecb5b0";
//const char* password = "QJZ3KDNLNMM3";
//const char* ssid = "Hadek25";
//const char* password = "12345678";
//const char* ssid = "Xiaomi 11T";
//const char* password = "aA123456";
//const char* ssid = "Banyai";
//const char* password = "24637954";

IPAddress staticIP(192, 168, 0, 200);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// Pin mapping for ESP32
const int analogPin_speed = 34; // ADC1_6 (GPIO34)
const int analogPin_angle = 35; // ADC1_7 (GPIO35)

WiFiClient client;
WebServer server(8080); // use WebServer for ESP32

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.config(staticIP, gateway, subnet);
  bool result = WiFi.config(staticIP, gateway, subnet);
  Serial.println(result ? "Static IP config applied" : "Static IP config failed");

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nESP32 IP Address: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/date", handleDate);
  server.begin();
  Serial.println("HTTP server started");

  pinMode(analogPin_speed, INPUT);
  pinMode(analogPin_angle, INPUT);
  

  struct SPEED_DATA* current_speed = (struct SPEED_DATA*)malloc(sizeof(struct SPEED_DATA));
  current_speed->speed = 0;
  current_speed->date = 0;
  current_speed->next = NULL;
  current_speed_ptr = current_speed;
  start_speed_ptr = current_speed_ptr;

  struct ANGLE_DATA* current_angle = (struct ANGLE_DATA*)malloc(sizeof(struct ANGLE_DATA));
  current_angle->angle = 0;
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
    Serial.println("Reconnecting WiFi...");
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
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
    current_speed->speed = analogRead(analogPin_speed);
    current_speed->date = time;
    current_speed->next = NULL;

    current_speed_ptr->next = current_speed;
    current_speed_ptr = current_speed_ptr->next;
    buffer -= 10;

    if (time % 1000 == 0) {
      struct ANGLE_DATA* current_angle = (struct ANGLE_DATA*)malloc(sizeof(struct ANGLE_DATA));
      current_angle->angle = analogRead(analogPin_angle);
      current_angle->next = NULL;
      
      current_angle_ptr->next = current_angle;
      current_angle_ptr = current_angle_ptr->next;
      buffer -= 6;
    }
  }
}
