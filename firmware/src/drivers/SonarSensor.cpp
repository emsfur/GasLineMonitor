#include "SonarSensor.h"

SonarSensor::SonarSensor(int triggerPin, int echoPin, int maxDistance) 
   : sonar(triggerPin, echoPin, maxDistance), prevState(false) {
}

bool SonarSensor::objectDetected()  {
    return sonar.ping_cm() > 0;
}

SensorEvent SonarSensor::poll() {
  bool newState = objectDetected();
  if (newState != prevState) {
    prevState = newState;
    return newState ? SensorEvent::ON_OCCUPIED : SensorEvent::ON_AVAILABLE;
  }
  return SensorEvent::NO_CHANGE;
}