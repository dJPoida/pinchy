/*============================================================================*\
 * Pinchy - Config
 * Peter Eldred 2020-06
 * 
 * This file contains all of the #define compiler flags and variables
 * which decide how Pinchy will run.
\*============================================================================*/
#ifndef CONFIG_H
#define CONFIG_H

/**
 * Serial Debug
 */
#define SERIAL_DEBUG          // Comment out this line to disable Serial Debug
#define SERIAL_BAUD 115200    // This is the baud rate that you select to listen to Pinchy's serial output when SERIAL_DEBUG is enabled

/**
 * Access Point Mode Settings
 */
#define AP_SSID "pinchy"      // The SSID and network domain name when the device is running in Access Point mode

/**
 * Web Server
 */
#define WEB_SERVER_PORT 80          // The port that the web server is served on.

/**
 * Config struct for storing and loading data from the SPIFFS partition
 */
struct Config {
  const char* apssid          = "pinchy"; // The SSID when the device is running in Access Point mode
  const char* mdnsAddress     = "pinchy"; // The address that clients can use to connect to the device without the IP (i.e. http://pinchy.local)
  String wifi_ssid            = "";       // The SSID of the wifi network that pinchy is configured to connect to
  String wifi_password        = "";       // The Password of the wifi network that pinchy is configured to connect to
  String ip_address           = "";       // The allocated IP address when connected to the WiFi
};

extern Config config;

#endif
