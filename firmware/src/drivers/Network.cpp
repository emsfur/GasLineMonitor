#include <Wifi.h>
#include <PubSubClient.h>
#include "Network.h"
#include "config/NetworkConfig.h"

WiFiClient espClient;
PubSubClient client(espClient);

NetworkState Network::initWifiConnection() {
  unsigned long start = millis();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Blocks until Wi-Fi is connected or timed out (time out length set in config/NetworkConfig.h)
  while (millis() - start < WIFI_CONNECT_TIMEOUT_MS) {
    Serial.println("Attempting WiFi connection....");
    
    if (WiFi.status() == WL_CONNECTED) {
      return NetworkState::CONNECTED;
      Serial.println("WiFi connection established.");
    }

    delay(250);
  }

  return NetworkState::DISCONNECTED;
}

NetworkState Network::initMQTTConnection() {
  unsigned long start = millis();

  client.setServer(MQTT_SERVER, MQTT_PORT);

  // Blocks until MQTT is connected or timed out (time out length set in config/NetworkConfig.h)
  while (millis() - start < MQTT_CONNECT_TIMEOUT_MS) {
    // Attempt to connect with credentials
    if (client.connect("ESP32Client", MQTT_USER, MQTT_PASS)) {
      Serial.println("MQTT connection established.");
      return NetworkState::CONNECTED;
    }
    delay(2000); // Wait before retrying
  }

  Serial.print("MQTT failed, rc=");
  Serial.print(client.state());
  Serial.println();

  return NetworkState::DISCONNECTED;
}

void Network::sendPayload(int payload) {
  // Ensure MQTT connection before publishing
  // If device is not connected to MQTT broker, attempt to reconnect and retry sending payload
  if (!client.connected()) {
    if (initMQTTConnection() == NetworkState::CONNECTED) {
      sendPayload(payload);
      return;
    }
  }

  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%d", payload);

  // Topic to publish under is defined in config/NetworkConfig.h
  client.publish(MQTT_TOPIC, buffer);

  Serial.println("Sent payload:");
  Serial.println(buffer);
}