#pragma once

#include <TFT_eSPI.h>

struct DisplayCoords {
    int x;
    int y;
};

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
        void init(int NUM_SONARS);

        /**
         * @brief Push a red/green icon to the target slot to indicate occupied or available.
         * @param slot index value of sensor correlating to a slot on the display
         * @param occupied true = occupied (red), false = available (green)
         */
        void displaySlotStatus(int slot, bool occupied);
    private:
        TFT_eSPI tft;
};