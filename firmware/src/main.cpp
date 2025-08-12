#include <Arduino.h>

#include "drivers/SonarSensor.h"
#include "drivers/Display.h"
#include "config/Pins.h"

SonarSensor sensor1(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // init sonar using config (TODO: account for multiple sonars)
Display display;

void setup() {
  Serial.begin(115200);   // Serial for debugging
  display.init();         // initialize TFT display
}

void loop() {
  // check the sensor for state changes and update display accordingly
  // handles transitions between occupied and available states
  switch (sensor1.poll())
  {
    case SensorEvent::ON_OCCUPIED:
      // mark the display red to indicate the slot is occupied
      display.markOccupied(true);
      break;

    case SensorEvent::ON_AVAILABLE:
      // mark the display green to indicate the slot is available
      display.markOccupied(false);
      break;
    
    default:
      // do nothing if state remains the same
      break;
  }
  
  delay(100);  // throttles the polling to reduce sensor/CPU stress   
}