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
#define WEB_SERVER_PORT 80    // The port that the web server is served on.

#endif
