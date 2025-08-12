#include "SonarSensor.h"

SonarSensor::SonarSensor(int triggerPin, int echoPin, int maxDistance) 
   : sonar(triggerPin, echoPin, maxDistance) {
}

bool SonarSensor::objectDetected()  {
    return sonar.ping_cm() > 0;
}