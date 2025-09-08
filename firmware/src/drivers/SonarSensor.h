#pragma once

#include <NewPing.h>

#include "config/SonarConfig.h" // Allows the direct use of max distance

/**
 * @brief Events emitted by SonarSensor::poll() to indicate confirmed state changes.
 */
enum class SensorEvent {
  NO_CHANGE,
  ON_OCCUPIED,
  ON_AVAILABLE
};

/**
 * @brief Abstraction over NewPing for occupancy detection.
 * 
 * Provides a polling API to simplify occupied/available events
 */
class SonarSensor {
    public:
        SonarSensor();

        /**
         * @brief Initializes NewPing object with defined trig and echo pin
         * 
         * NewPing object is stored via for the entire lifetime of parent SonarSensor object
         * @note Max distance parameter is pulled from SonarConfig.h
         */
        void init(uint8_t trig_pin, uint8_t echo_pin);

        /**
         * @brief Checks if the sensor detects an object within max range.
         * @return True if an object is detected within MAX_DISTANCE_CM, false otherwise.
         */
        bool objectDetected();

        /**
         * @brief Checks for a change in object detection state.
         * 
         * If a raw state change persists longer than CONFIRMATION_WINDOW_MS, this returns:
         * 
         *  - SensorEvent::ON_OCCUPIED when rawAvailable -> occupied is confirmed.
         * 
         *  - SensorEvent::ON_AVAILABLE when occupied -> available is confirmed.
         * 
         *  - SensorEvent::NO_CHANGE otherwise.
         *
         * @return SensorEvent describing the confirmed transition.
         */
        SensorEvent poll();
    private:
        // Reserve enough memory for exactly one NewPing object; Constructed in init()
        alignas(NewPing) uint8_t sonarStorage[sizeof(NewPing)];
        NewPing* sonarPtr; // Points into sonarStorage

        bool lastRecorded;            // Tracks whether the sensor previously detected an object
        bool isOccupied;              // Tracks the confirmed state of the current sensor slot
        unsigned long debounceTimer;  // Stores the length of persistance for the current state
};