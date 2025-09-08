#pragma once

/**
 * @brief Represents the connection state of the device.
 * 
 * Used by WiFi and MQTT connection functions to signal current connectivity status
 */
enum class NetworkState {
  CONNECTED,
  DISCONNECTED
};


/**
 * @brief Manages WiFi and MQTT connectivity.
 *
 * Provides connection management and a simple API for publishing data over MQTT.
 */
class Network {
public:
  /**
   * @brief Initializes internet connection
   * 
   * @note Function uses WiFi credentials and time out length defined in config/NetworkConfig.h
   */
  NetworkState initWifiConnection();

  /**
   * @brief Initializes connection to MQTT broker 
   * 
   * @note Function uses MQTT credentials and time out length defined in config/NetworkConfig.h
   * 
   * @warning An active internet connection is required
   */
  NetworkState initMQTTConnection();
  
  /**
   * @brief Sends a raw data payload to defined MQTT server.
   * 
   * If disconnected, attempts one reconnect before skipping payload.
   * 
   * @param payload Integer value to send 
   * 
   * @warning An active internet connection and mqtt broker connection is required
   */
  void sendPayload(int payload);

};