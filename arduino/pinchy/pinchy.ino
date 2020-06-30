
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
 * Protection - Prevent this code from running on anything other than an ESP32
 */
#ifndef ESP32
#error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif


/**
 * Includes
 */
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include "config.h"
#include "pinchyFS.h"
#include "wifiEngine.h"
#include "brain.h"


/**
 * Global Varibles
 */
const char* firmwareVersion = "0.0.1";      // Firmware Version
AsyncWebServer webServer(WEB_SERVER_PORT);  // Pinchy's Web Server for serving the control code
DNSServer dnsServer;                        // A DNS Server for use when in Access Point mode
Config config;                              // Pinchy's configuration
PinchyFS pinchyFS = PinchyFS();             // Pinchy's File System
Brain brain = Brain();                      // Pinchy's Brain
WiFiEngine wifiEngine = WiFiEngine();       // Pinchy's WiFi engine


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
    // TODO: at some point perform a "HALT" with a red-flashing light
    return;
  }

  // Initialise the WiFi Engine
  // This will automatically attempt to connect to a pre-configured
  // WiFi hotspot and if unable to do so will broadcast Pinchy's Access Point
  if (!wifiEngine.init(&webServer, &dnsServer)) {
    // Failed to initialise the WiFi hotspot. Oh well. Bail.
    // TODO: at some point perform a "HALT" with a red-flashing light
    return;
  }

  // Initialise the Brain
  brain.init();
}


/**
 * Main Loop
 */
void loop() {
  // If the wifiEngine is in Access Point mode, process DNS requests.
  if (wifiEngine.wifiEngineMode == WEM_AP) {
    dnsServer.processNextRequest();
  }
}
