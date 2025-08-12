#include <Arduino.h>

#include "drivers/SonarSensor.h"
#include "drivers/Display.h"
#include "config/Pins.h"

SonarSensor sensor1(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // init sonar using config (TODO: account for multiple sonars)
Display display;

bool prevState;
bool currState;

void setup() {
  Serial.begin(115200);   // Serial for debugging
  display.init();         // initialize TFT display

  prevState = false;      // Assume no object detected at startup
}

void loop() {
  currState = sensor1.objectDetected();

  // on state change of undetected to detected object
  if (currState && !prevState) {
    // fills the display with red to mark as used
    display.markUsed(true);
  }
  // on state change of detected to undetected
  else if (!currState && prevState) {
    // fill the display with green to show availability
    display.markUsed(false);
  }

  prevState = currState; // update for next iteration

  delay(100);
}