#include "Display.h"

#include "assets/fuel.h"
#include "assets/mqtt.h"
#include "assets/wifi.h"

#include "config/DisplayConfig.h"

void Display::init(int numSonars) {
  tft.init();
  tft.fillScreen(TFT_BLACK);  // System starts up with the slot available
  tft.setSwapBytes(true);     // Enables the use of image rendering

  // Fill all slots as available on TFT display
  for (int i = 0; i < numSonars; i++) {
    setSlotStatus(i, false);
  }

  // Begin display with red network icons
  displayWifiStatus(false);
  displayMQTTStatus(false);
}

void Display::setSlotStatus(int slot, bool occupied) {
  // Center fuel icon horizontally. vertical padding/placement is defined by padding value from config and slot value
  // TODO: cache values to save resources from repeated computations
  int startingX = (TFT_WIDTH / 2) - (FUEL_ICON_WIDTH / 2);
  int startingY = VERTICAL_PADDING_PX + ( (FUEL_ICON_HEIGHT + VERTICAL_PADDING_PX) * slot );

  // Render red (occupied) or green (available) fuel icon
  if (occupied) {
    tft.pushImage(startingX, startingY, FUEL_ICON_WIDTH, FUEL_ICON_HEIGHT, fuel_red);
  }
  else {
    tft.pushImage(startingX, startingY, FUEL_ICON_WIDTH, FUEL_ICON_HEIGHT, fuel_green);
  }
}

void Display::displayWifiStatus(bool connected) {
  // Place WiFi icon at bottom-left corner of display
  int xPos = 10;
  int yPos = tft.height() - WIFI_ICON_HEIGHT;

  // Draw WiFi status icon at bottom-left corner of display
  // Green when connected, red when disconnected
  if (connected) {
    tft.pushImage(xPos, yPos, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, wifi_green);
  }
  else {
    tft.pushImage(xPos, yPos, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, wifi_red);
  }
}

void Display::displayMQTTStatus(bool connected) {
  // Place MQTT icon at bottom-right corner of display
  int xPos = 10;
  int yPos = tft.height() - WIFI_ICON_HEIGHT;

  // Draw MQTT status icon at bottom-right corner of display
  // Uses green when connected, red when disconnected
  if (connected) {
    tft.pushImage(tft.width() - 10 - MQTT_ICON_WIDTH, tft.height() - MQTT_ICON_HEIGHT, MQTT_ICON_WIDTH, MQTT_ICON_HEIGHT, mqtt_green);
  }
  else {
    tft.pushImage(tft.width() - 10 - MQTT_ICON_WIDTH, tft.height() - MQTT_ICON_HEIGHT, MQTT_ICON_WIDTH, MQTT_ICON_HEIGHT, mqtt_red);
  }
}