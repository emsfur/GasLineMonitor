#include "SonarSensor.h"

SonarSensor::SonarSensor() : sonarPtr(nullptr), lastRecorded(false), debounceTimer(0), isOccupied(false) {
}

void SonarSensor::init(uint8_t trig_pin, uint8_t echo_pin) {
  // Construct the NewPing object in the reserved buffer via placement-new
  sonarPtr = new (&sonarStorage) NewPing(trig_pin, echo_pin, MAX_DISTANCE_CM);
}

bool SonarSensor::objectDetected()  {
  // Returns distance in cm; 0 indicates out-of-range/no detection.
  return sonarPtr->ping_cm() > 0;
}

SensorEvent SonarSensor::poll() {
  bool rawOccupied = objectDetected();
  unsigned long now = millis();

  // Reset timer if state changes to signify if the state change hasn't lasted long enough
  if (rawOccupied != lastRecorded) {
    lastRecorded = rawOccupied;
    debounceTimer = now;
  }

  // Only treat a change as confirmed if it persists longer than CONFIRMATION_WINDOW_MS; Prevents optimistic state change
  if (now - debounceTimer >= CONFIRMATION_WINDOW_MS) {
    // confirm that the current state (rawOccupied) has changed from the confirmed state (isOccupied)
    if (rawOccupied && !isOccupied) {
      isOccupied = true;
      return SensorEvent::ON_OCCUPIED; // returns occupied if it wasn't previously
    }
    else if (!rawOccupied && isOccupied) {
      isOccupied = false;
      return SensorEvent::ON_AVAILABLE; // returns available if it wasn't previously
    }
  }

  // Send off as no change if a state change hasn't been confirmed
  return SensorEvent::NO_CHANGE;
}