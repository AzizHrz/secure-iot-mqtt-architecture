#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Paste content of ca.crt here
const char* ca_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDOTCCAiGgAwIBAgIUPt4IvGvpLPZqx9jMBW0rwi13leIwDQYJKoZIhvcNAQEL
BQAwLDEPMA0GA1UEAwwGSW9ULUNBMQwwCgYDVQQKDANUUDQxCzAJBgNVBAYTAlRO
MB4XDTI2MDMzMTIzMDA0NVoXDTMxMDMzMTIzMDA0NVowLDEPMA0GA1UEAwwGSW9U
LUNBMQwwCgYDVQQKDANUUDQxCzAJBgNVBAYTAlROMIIBIjANBgkqhkiG9w0BAQEF
AAOCAQ8AMIIBCgKCAQEAtJzGotOCjQMvKj2392yy/jw3XfrLfbF+yK1kwbGS+YH6
7pPYxwOf0/7GwUUMobC9JnorRoBeQnXqITHOQhkR5ELF5MPy3vENwxo6WyNMQCRP
CvQzReO8kyIlh3uXGPdgtdSTLR/iMFlcbSdEM8UaE8iOAGlGOZo6gHU7GuXCYfyA
0nZgI+0kj5ukMhMhC+kkMzLDkphv0QdZFmlKNnA6jn68EvikMidCaD2LVlD05EJd
+WKAwA2IngziKr+GlDCjUma29JYPPFAZssWMU2iWaGMkHuIqxwWmz1RprRM3DQV/
EfsDw7Cy5AVVPFV/zTJV9SMp6uP1EO2OPaXyVhyNVwIDAQABo1MwUTAdBgNVHQ4E
FgQU/zkC5Eqmy0J38GWjD2+OUVqfsxYwHwYDVR0jBBgwFoAU/zkC5Eqmy0J38GWj
D2+OUVqfsxYwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAQVFy
vk3uDQZqAnHhlzqGqVr8clUeqvO8CJRbE3+gR9tpYh8aipkkFqOcnqwUNQ8eamRh
gNhg0higTE+pd8ag7H69Ac6KBkmZ/lygtVGKplC4GwxysoxIcbeg+EfXoHoGYzy+
It18TPRCocRL4zllW347N5kMTqkud9ENMJuiQ+xhZ/39f6zPfWS964IIHGt4ATJZ
vNUip0pUXyN/WNWFioeKA16wU6U79BEoeXA0TpTt/qYF/NObjEk7XD2zMuKlpb5l
GE/wPpAt0eXFvj6SWAh7GxhD/XMBJJYnWwYAW3/kAb6kjZvkbuNCnKuDYcEQ2kOF
GifxC+oUmJz0RKtNmQ==
-----END CERTIFICATE-----

)EOF";

const char* ssid        = "Aziz_s_iPhone";
const char* password    = "it-s4you";
const char* mqtt_server = "172.20.10.2";  // ← your Debian IP
const int   mqtt_port   = 8883;

WiFiClientSecure espClient;
PubSubClient client(espClient);

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi OK");
}

void connectMQTT() {
  espClient.setCACert(ca_cert);
  while (!client.connected()) {
    if (client.connect("esp32_sensor", "esp32", "esp32pass")){
      Serial.println("MQTT+TLS connected!");
    } else {
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
  String payload = "{\"temp\":" + String(temp,1) + "}";
  client.publish("iot/sensor/data", payload.c_str());
  
  Serial.println("Published (TLS): " + payload);
  delay(5000);
}