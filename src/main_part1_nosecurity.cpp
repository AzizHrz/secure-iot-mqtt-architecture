#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid        = "Aziz_s_iPhone";
const char* password    = "it-s4you";
const char* mqtt_server = "172.20.10.2";  // ← your Debian IP
const int   mqtt_port   = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    if (client.connect("esp32_sensor")) {
      Serial.println("Connected!");
    } else {
      Serial.print("Failed rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
  client.setServer(mqtt_server, mqtt_port);

   
}

void loop() {
  if (!client.connected()) connectMQTT();
  client.loop();

  float temp = 20.0 + random(0, 150) / 10.0;
  float hum  = 40.0 + random(0, 400) / 10.0;
unsigned long runingTime = millis() ;
String payload = "{\"temp\":"        + String(temp, 1)
               + ",\"hum\":"         + String(hum, 1)
               + ",\"runingTime\":"  + String(runingTime) + "}";

  client.publish("secureMQTT/sensor/data", payload.c_str());
  Serial.println("Published: " + payload);
 
  delay(5000);
}