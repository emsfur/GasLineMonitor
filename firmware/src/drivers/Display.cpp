#include "Display.h"

void Display::init() {
    tft.init();
    tft.fillScreen(TFT_GREEN);  // assumes the system starts up with the slot available
    tft.setSwapBytes(true);     // enables the use of image rendering
}

void Display::markOccupied(bool occupied) {
    // if the slot is occupied, make display red to signify use
    if (occupied) {
        tft.fillScreen(TFT_RED);
    }
    // if the slot is not occupied, make display green to signify availability
    else {
        tft.fillScreen(TFT_GREEN);
    }
}