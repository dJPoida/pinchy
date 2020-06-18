/*============================================================================*\
 * Pinchy - WifiClient
 * Peter Eldred 2020-06
 * 
 * When Pinchy is configured to connect to a WiFi hotspot, it connects as a 
 * client. This unit is initialised when pinchy is in WiFi client mode.
\*============================================================================*/
#include "wifiClient.h"


/**
 * Constructor
 */
WifiClient::WifiClient(){}


/**
 * Initialise
 */
bool WifiClient::init(AsyncWebServer *webServer) {
  #ifdef SERIAL_DEBUG
  Serial.println(" % Initialising WiFi in Client Mode...");
  #endif

  // Keep a pointer to the AsyncWebServer
  _webServer = webServer;
  
  // TODO: at some point actually attempt to connect to the configured wifi 
  return false;

  #ifdef SERIAL_DEBUG
  Serial.println(" % WiFi in client Mode initialised.");
  #endif

  return true;
}
