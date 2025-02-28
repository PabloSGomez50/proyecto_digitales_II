#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define PIN_WRITE_ACTIVE 2

// Replace with your network credentials
const char* ssid_ap     = "DII-Access-Point";
const char* password_ap = "123456789";
const char* ssid_wifi     = "**********";
const char* password_wifi = "**********";

const char* apDomain = "lidar";
// Tiempo constantes
unsigned long start_time;
String ipaddress = "";

#define MAX_SIZE_JSON 512
ESP8266WebServer server(80);

struct laser_data {
  uint16_t range;
  uint16_t distance;
  uint16_t signal_rate;
  uint16_t ambient_light;
  uint16_t spad_num;
  uint16_t mot_angle;
  uint16_t mot_lap;
};
#define DATA_SIZE 128
struct laser_data data[DATA_SIZE] = {0};
uint8_t data_index = 0;

#define BUFFER_SIZE 128
#define MAX_USART_MS 500
char buffer[BUFFER_SIZE] = {0};
uint16_t usart_index = 0;
uint8_t usart_msg_ready = 0;

void setupServerRoutes();
void handleRoot();
void handleMessage();
void handleStart();
void handleIdle();
void handleBuffer();
void handleSensor();


uint8_t readSerial();
struct laser_data parseData();
void connectToWiFi();
void startAPMode();
void setupDNS();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_WRITE_ACTIVE, OUTPUT);
  digitalWrite(PIN_WRITE_ACTIVE, LOW);
  
  connectToWiFi();
  
  start_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  MDNS.update();
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '$') {
      buffer[0] = c;
      buffer[1] = '\0';
      usart_index = 1;
      readSerial();
      if (data_index < DATA_SIZE) {
        data[data_index] = parseData();
        data_index++;
      } else {
        Serial.write("IDLE\n", 5);
      }
    }
  }
}



uint8_t readSerial() {
  unsigned long start_read = millis();
  while ((millis() - start_read) < MAX_USART_MS) {
    while (Serial.available()) {
      char c = Serial.read();
      // Ignore carriage return and buffer overflow
      if (c == '\r') continue;

      if (c == '\n') {
        buffer[usart_index] = '\0';  // Null-terminate the buffer
        usart_index = 0;             // Reset the buffer index
        usart_msg_ready = 1;
        return 1;                 // Message is complete
      }

      // Store character if there's space in the buffer
      if (usart_index < (BUFFER_SIZE - 1)) {
        buffer[usart_index++] = c;
        buffer[usart_index] = '\0';
      }
    }
    yield(); // Avoid watchdog reset
  }

  usart_index = 0;
  return 0;
}


struct laser_data parseData() {
  struct laser_data content;
  if (buffer[0] != '$') {
    return content;
  }

  // Pointers for parsing
  char *ptr = buffer + 1; // Skip '$'
  char *endPtr;

  // Extract and convert each field
  content.range = strtoul(ptr, &endPtr, 10);
  if (*endPtr != ',') return content; ptr = endPtr + 1;

  content.distance = strtoul(ptr, &endPtr, 10);
  if (*endPtr != ',') return content; ptr = endPtr + 1;

  content.signal_rate = strtoul(ptr, &endPtr, 10);
  if (*endPtr != ',') return content; ptr = endPtr + 1;

  content.ambient_light = strtoul(ptr, &endPtr, 10);
  if (*endPtr != ',') return content; ptr = endPtr + 1;

  content.spad_num = strtoul(ptr, &endPtr, 10);
  if (*endPtr != ',') return content; ptr = endPtr + 1;

  content.mot_angle = strtoul(ptr, &endPtr, 10);
  if (*endPtr != ',') return content; ptr = endPtr + 1;

  content.mot_lap = strtoul(ptr, &endPtr, 10);
  return content;
}

void setupDNS() {
  if (!MDNS.begin(apDomain)) {
    #if SERIAL_ON
    Serial.println("Error setting up MDNS responder!");
    #endif
    while(1) {
      delay(1000);
    }
  }
  #if SERIAL_ON
  Serial.println("mDNS responder started");
  #endif
}


void startAPMode() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid_ap, password_ap);
  ipaddress = WiFi.softAPIP().toString();
  #if SERIAL_ON
  Serial.print("AP IP address: ");
  Serial.println(ipaddress);
  #endif

  setupDNS();

  setupServerRoutes();
}

void connectToWiFi() {
  WiFi.begin(ssid_wifi, password_wifi);
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 5000) {
    delay(500);
    #if SERIAL_ON
    Serial.print(".");
    #endif
  }

  if (WiFi.status() == WL_CONNECTED) {
    #if SERIAL_ON
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
    #endif
    ipaddress = WiFi.localIP().toString();
    digitalWrite(PIN_WRITE_ACTIVE, HIGH);
    setupDNS();
    setupServerRoutes();
    // wifi_connected = true;
  } else {
    #if SERIAL_ON
    Serial.println("Failed to connect to WiFi");
    #endif
    startAPMode();
  }
}

// Routers
void setupServerRoutes() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/msg", HTTP_POST, handleMessage);
  server.on("/active", HTTP_POST, handleStart);
  server.on("/idle", HTTP_POST, handleIdle);
  server.on("/buffer", HTTP_GET, handleBuffer);
  server.on("/sensor", HTTP_GET, handleSensor);
  server.begin();
}

void handleRoot() {
  String text = "Esp01 Server activo en ip: " + ipaddress + ", Necesitaas mas informacion?";

  server.send(200, "text/plain", text);
}

void handleSensor() { 
  DynamicJsonDocument doc(1024);

  JsonArray distanceArray = doc.createNestedArray("distance");
  JsonArray motAngleArray = doc.createNestedArray("mot_angle");
  JsonArray motLapArray = doc.createNestedArray("mot_lap");

  for (int i = 0; i < data_index; i++) {
    distanceArray.add(data[i].distance);
    motAngleArray.add(data[i].mot_angle);
    motLapArray.add(data[i].mot_lap);
  }

  data_index = 0;
  // Send JSON response
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleMessage() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", R"({"error": "Invalid request"})");
    return;
  }

  DynamicJsonDocument doc(MAX_SIZE_JSON);
  DeserializationError error = deserializeJson(doc, server.arg("plain"));
  if (error) {
    server.send(401, "application/json", "{\"error\": \"Invalid JSON\"}");
    return;
  }
  const char* message = doc["message"];
  if (!message) {
    server.send(400, "application/json", R"({"error": "Missing message"})");
    return;
  }

  Serial.write(message, strlen(message));

  if (readSerial() == 1) {
    String response = R"({"status": "Message sent with response", "msg": ")" + String(buffer) + "\"}";
    server.send(200, "application/json", response);
  }
  else
    server.send(200, "application/json", R"({"status": "Message sent without response"})");
  
}

void handleBuffer() {
  server.send(200, "application/json", "{\"status\": \"Buffer with len " + String(strlen(buffer)) + "\", \"buffer\": \"" + String(buffer) + "\"}");
}

void handleStart() {
  Serial.write("ACTIVE\n", 7);

  if (readSerial() == 1)
    server.send(200, "application/json", "{\"status\": \"LPC845 started\", \"msg\": \"" + String(buffer) + "\"}");
  else
    server.send(200, "application/json", "{\"status\": \"LPC845 started wo msg\"}");
}

void handleIdle() {
  Serial.write("IDLE\n", 5);
  
  if (readSerial() == 1)
    server.send(200, "application/json", "{\"status\": \"LPC845 stopped\", \"msg\": \"" + String(buffer) + "\"}");
  else
    server.send(200, "application/json", "{\"status\": \"LPC845 stopped wo msg\"}");
}
