#include "Display.h"

void Display::init() {
    tft.init();
    tft.fillScreen(TFT_GREEN);  // System starts up with the slot available
    tft.setSwapBytes(true);     // Enables the use of image rendering
}

void Display::markOccupied(bool occupied) {
    // If the slot is occupied, fill the display with red to signify use
    if (occupied) {
        tft.fillScreen(TFT_RED);
    }
    // if the slot is not occupied, fill the display with green to signify availability
    else {
        tft.fillScreen(TFT_GREEN);
    }
}