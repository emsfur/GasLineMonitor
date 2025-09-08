#include <Arduino.h>

#include "drivers/SonarSensor.h"
#include "drivers/Display.h"
#include "config/SonarConfig.h"
#include "drivers/Network.h"

// Number of sonars derived from config
constexpr int NUM_SONARS = (sizeof(sonarPins)/sizeof(sonarPins[0]));
SonarSensor sonars[NUM_SONARS];

Display display;
Network network;

NetworkState wifiConnected;
NetworkState mqttConnected;

bool publishPending = false;  // Flag for only sending MQTT messages on data change
int numOccupied;              // Tracks the number of occupied slots at the station

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_SONARS; i++) {
    // Initialize each sensor's NewPing instance with it's trigger/echo pins
    // This binds the hardware layout (from SonarConfig) to the runtime logic
    sonars[i].init(sonarPins[i].trig_pin, sonarPins[i].echo_pin);
  }

  display.init(NUM_SONARS);

  // Attempt WiFi connection (blocking until success or timeout)
  // Timeout settings are configured in config/NetworkConfig.h
  wifiConnected = network.initWifiConnection();
  if (wifiConnected == NetworkState::CONNECTED) {
    display.displayWifiStatus(true);
  }
  
  // Attempt MQTT Broker connection (blocking until success or timeout)
  // Timeout settings are configured in config/NetworkConfig.h
  mqttConnected = network.initMQTTConnection();
  if (mqttConnected == NetworkState::CONNECTED) {
    display.displayMQTTStatus(true);
  }

  numOccupied = 0;
}

void loop() {
  for (int i = 0; i < NUM_SONARS; i++) {
    SonarSensor &sensor = sonars[i];

    // Check the sensor for state changes and update display accordingly
    // Handles transitions between occupied and available states
    switch (sensor.poll())
    {
      case SensorEvent::ON_OCCUPIED:
        // Display updates: true = occupied (red)
        display.setSlotStatus(i, true);

        
        numOccupied++;
        publishPending = true;  // Set flag to signify that change needs to be published

        break;

      case SensorEvent::ON_AVAILABLE:
        // Display updates: false = available (green)
        display.setSlotStatus(i, false);

        
        numOccupied--;
        publishPending = true;  // Set flag to signify that change needs to be published

        break;
      
      default:
        // Do nothing if state remains the same
        break;
    }
    
    delay(100);  // Throttles the polling to reduce sensor/CPU stress   
  }

  // If any slot state changed, publish the new utilization percentage
  // Uses intial mqtt connection state to determine if payload can be sent (avoids repeating reconnection attempts)
  if (mqttConnected == NetworkState::CONNECTED && publishPending) {
    int utilized = (numOccupied * 100) / (NUM_SONARS);
    network.sendPayload(utilized);

    publishPending = false;
  }
}