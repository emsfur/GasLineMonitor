#pragma once

#include <TFT_eSPI.h>

/**
 * @brief Display helper to manage occupancy state visulizations.
 * 
 * Current API is single-slot oriented.
 */
class Display {
    public:
        /**
         * @brief Initializes the display hardware and wipes screen.
         * Call during setup()
         */
        void init();

        /**
         * @brief Fill the display to indicate occupied or available.
         * @param occupied true = occupied (red), false = available (green)
         * 
         * @note Current implementation repaints entire screen, overwriting input from other sensors.
         */
        void markOccupied(bool occupied);
    private:
        TFT_eSPI tft;
};