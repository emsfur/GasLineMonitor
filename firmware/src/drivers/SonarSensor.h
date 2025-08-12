#pragma once

#include <NewPing.h>

enum class SensorEvent {
  NO_CHANGE,
  ON_OCCUPIED,
  ON_AVAILABLE
};

class SonarSensor {
    public:
        SonarSensor(int triggerPin, int echoPin, int maxDistance);

        /**
         * @brief Checks if the sensor detects an object within max range.
         */
        bool objectDetected();

        /**
         * @brief Checks for a change in object detection state.
         *
         * Polls the sensor to determine if an object is currently detected.
         * Compares with the previous state and returns a SensorEvent indicating
         * whether the state changed to OCCUPIED, AVAILABLE, or remains NO_CHANGE.
        */
        SensorEvent poll();
    private:
        NewPing sonar;
        bool prevState;
};