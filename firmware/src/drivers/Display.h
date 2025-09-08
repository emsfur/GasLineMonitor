#pragma once

#include <TFT_eSPI.h>

struct DisplayCoords {
  int x;
  int y;
};

/**
 * @brief Wrapper around TFT_eSPI for application-specific display logic.
 *
 * Initializes slot indicators on the display and manages their state (red = occupied, green = available). 
 * 
 * Also renders WiFi and MQTT connection status using the same mechanism (red = disconnected, green = connected).
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
    void setSlotStatus(int slot, bool occupied);

    /**
     * @brief Visualize the WiFi connectivity status of the device (bottom left corner)
     * @param connected true means device is connected to MQTT broker (green icon), otherwise disconnected (red icon)
     */
    void displayWifiStatus(bool connected);

    /**
     * @brief Visualize the MQTT connectivity status of the device (bottom right corner)
     * @param connected true means device is connected to MQTT broker (green icon), otherwise disconnected (red icon)
     */
    void displayMQTTStatus(bool connected);
  private:
    TFT_eSPI tft;
};