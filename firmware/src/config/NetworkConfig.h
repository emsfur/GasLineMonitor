#pragma once

constexpr unsigned long WIFI_CONNECT_TIMEOUT_MS = 5000;
constexpr unsigned long MQTT_CONNECT_TIMEOUT_MS = 10000;

// WiFi Credentials
const char* WIFI_SSID = "YourSSID";
const char* WIFI_PASSWORD = "YourPassword";

// MQTT Broker Credentials/Config
const char* MQTT_SERVER = "broker.example.com";
const int   MQTT_PORT = 1883;
const char* MQTT_USER = "mqtt";
const char* MQTT_PASS = "password";

const char *MQTT_TOPIC = "fuel_station";