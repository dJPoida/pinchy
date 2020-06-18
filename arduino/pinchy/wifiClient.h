/*============================================================================*\
 * Pinchy - WifiClient
 * Peter Eldred 2020-06
 * 
 * When Pinchy is configured to connect to a WiFi hotspot, it connects as a 
 * client. This unit is initialised when pinchy is in WiFi client mode.
\*============================================================================*/
#ifndef WIFICLIENT_H
#define WIFICLIENT_H

#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"

class WifiClient {
  public:
    WifiClient();
    bool init(AsyncWebServer *webServer);

  private:
    AsyncWebServer *_webServer;
};

extern WifiClient wifiClient;

#endif
