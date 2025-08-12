#include "Display.h"

void Display::init() {
    tft.init();
    tft.fillScreen(TFT_BLACK);  // black screen to recognize system start 
    tft.setSwapBytes(true);     // enables the use of images on display
}

void Display::markUsed(bool marked) {
    if (marked) {
        tft.fillScreen(TFT_RED);
    }
    else {
        tft.fillScreen(TFT_GREEN);
    }
}