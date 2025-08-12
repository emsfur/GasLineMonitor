#pragma once

#include <TFT_eSPI.h>

class Display {
    public:
        /**
         * @brief Initializes the display hardware.
         * Sets up the TFT display and clears the screen.
         */
        void init();
        
        /**
         * @brief Fills the display with a color to indicate usage.
         * Red means occupied, green means available.
         */
        void markUsed(bool marked);
    private:
        TFT_eSPI tft;
};