#include "Display.h"

#include "assets/fuel.h"
#include "config/DisplayConfig.h"

void Display::init(int NUM_SONARS) {
    tft.init();
    tft.fillScreen(TFT_BLACK);  // System starts up with the slot available
    tft.setSwapBytes(true);     // Enables the use of image rendering

    // fill all slots as available on TFT display
    for (int i = 0; i < NUM_SONARS; i++) {
        displaySlotStatus(i, false);
    }
}

void Display::displaySlotStatus(int slot, bool occupied) {
    // Calculates the image location on screen based on screen/image dimensions and config values
    // TODO: cache values to save resources from repeated computations
    int startingX = (TFT_WIDTH / 2) - (ICON_WIDTH / 2);
    int startingY = VERTICAL_PADDING_PX + ( (ICON_HEIGHT + VERTICAL_PADDING_PX) * slot );

    if (occupied) {
        // place fuel icon in the horozontal center, with 30px vertical spacing between each icon
        tft.pushImage(startingX, startingY, ICON_WIDTH, ICON_HEIGHT, fuel_red);
    }
    else {
        tft.pushImage(startingX, startingY, ICON_WIDTH, ICON_HEIGHT, fuel_green);
    }
}