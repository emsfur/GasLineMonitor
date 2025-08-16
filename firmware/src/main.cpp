#include <Arduino.h>

#include "drivers/SonarSensor.h"
#include "drivers/Display.h"
#include "config/SonarConfig.h"

// Number of sonars derived from config
constexpr int NUM_SONARS = (sizeof(sonarPins)/sizeof(sonarPins[0]));
SonarSensor sonars[NUM_SONARS];

Display display;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_SONARS; i++) {
    //  Initialize each sensor's NewPing instance using pins from SonarConfig
    sonars[i].init(sonarPins[i].trig_pin, sonarPins[i].echo_pin);
  }

  display.init(NUM_SONARS); // Initialize TFT display and icon coordinates
}

void loop() {
  for (int i = 0; i < NUM_SONARS; i++) {
    SonarSensor &sensor = sonars[i];

    // Check the sensor for state changes and update display accordingly
    // Handles transitions between occupied and available states
    switch (sensor.poll())
    {
      case SensorEvent::ON_OCCUPIED:
        // Mark the display red to indicate the slot is occupied
        display.displaySlotStatus(i, true);
        break;

      case SensorEvent::ON_AVAILABLE:
        // Mark the display green to indicate the slot is available
        display.displaySlotStatus(i, false);
        break;
      
      default:
        // Do nothing if state remains the same
        break;
    }
    
    delay(100);  // Throttles the polling to reduce sensor/CPU stress   
  }
}