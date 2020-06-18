/*============================================================================*\
 * Pinchy - WifiAPServer
 * Peter Eldred 2020-06
 * 
 * When Pinchy cannot connect to a configured WiFi hotspot, it instantiates its
 * own hotspot so that a user can connect to it and issue details about the 
 * local WiFi network. This class controls those attributes of the WiFi Access
 * Point server.
\*============================================================================*/
#ifndef WIFIAPSERVER_H
#define WIFIAPSERVER_H

#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"

class WifiAPServer {
  public:
    WifiAPServer();
    bool init(AsyncWebServer *webServer);

  private:
    AsyncWebServer *_webServer;
    char* _apSSID;      // The SSID of the Access Point to broadcast
    IPAddress _IP;      // The IP address of the Access Point

    void initRoutes();                            // Initialise the AP mode Web Server routes
    static String templateProcessor(const String& var);  // Used when serving HTML files to replace key variables in the HTML
};

extern WifiAPServer wifiAPServer;

#endif
