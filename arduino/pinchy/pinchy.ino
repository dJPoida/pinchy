/*============================================================================*\
 * Pinchy
 * Author: Peter Eldred
 * Date: 2020-06
 * GitHub: TODO
 * Description: Pinchy is a STEM robot geared toward younger children (ages 4 -> 10)
 *              with varying levels of skill and interest required to interact.
 * 
 * Hardware
 *  - ESP32 Dev Module (30 Pin)
 * 
 * Dependencies:
 *  - Arduino ESP32 File System Uploader (https://github.com/me-no-dev/arduino-esp32fs-plugin)
 *  - Async TCP Library for ESP32 Arduino (https://github.com/me-no-dev/AsyncTCP)
 *  - ESP Async Web Server (https://github.com/me-no-dev/ESPAsyncWebServer)
\*============================================================================*/


/**
 * Includes
 */
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include "config.h"
#include "pinchyFS.h"
#include "wifiClient.h"
#include "wifiAPServer.h"
#include "brain.h"


/**
 * Global Varibles
 */
const char* firmwareVersion = "0.0.1";      // Firmware Version
AsyncWebServer webServer(WEB_SERVER_PORT);     // Pinchy's Web Server for serving the control code
PinchyFS pinchyFS = PinchyFS();             // Pinchy's File System
Brain brain = Brain();                      // Pinchy's Brain
WifiClient wifiClient = WifiClient();       // Pinchy's WiFi client
WifiAPServer wifiAPServer = WifiAPServer(); // Pinchy's WiFi Access Point server (for when the WiFi client fails)



// Prevent this code from running on anything other than an ESP32
#ifndef ESP32
#error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif


/**
 * Setup
 */
void setup() {
  // Serial Initialisation
  #ifdef SERIAL_DEBUG
  Serial.begin(SERIAL_BAUD);
  Serial.println("");
  Serial.print("Pinchy v");
  Serial.println(firmwareVersion);
  #endif
    
  // Initialise the PinchyFS
  if (!pinchyFS.init()) {
    // Failed to initialise the File System. Oh well. Bail.
    return;
  }

  // Initialise the WiFi
  if (!wifiClient.init(&webServer)) {
    
    // Failed to initialise the WiFi - Begin broadcasting a hotspot
    if (!wifiAPServer.init(&webServer)) {
      // Failed to initialise the WiFi hotspot. Oh well. Bail.
      return;
    }
  } 

  // Only initialise the remainder of the robot when the WiFi is running in client mode
  else {
    // Initialise the Brain
    brain.init();  
  }
}


/**
 * Main Loop
 */
void loop() {
}
