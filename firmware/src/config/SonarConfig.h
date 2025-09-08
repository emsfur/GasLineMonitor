#pragma once

constexpr unsigned int MAX_DISTANCE_CM = 3; // Defines distance limit to trigger object detection

constexpr unsigned long CONFIRMATION_WINDOW_MS = 2000; // How long sensor state must remain unchanged before updating display

struct SonarPins {
  uint8_t trig_pin;
  uint8_t echo_pin;
};

// Configure trig and echo pins for each sonar sensor
constexpr SonarPins sonarPins[] = {
  {2, 32},
  {15, 33}
};




