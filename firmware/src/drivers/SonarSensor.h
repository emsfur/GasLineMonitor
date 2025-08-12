#pragma once

#include <NewPing.h>

class SonarSensor {
    public:
        SonarSensor(int triggerPin, int echoPin, int maxDistance);

        /**
         * @brief Returns raw boolean if the sensor detects an object within range.
         */
        bool objectDetected();
    private:
        NewPing sonar;
};