/*============================================================================*\
 * Pinchy - WifiAPServer
 * Peter Eldred 2020-06
 * 
 * When Pinchy cannot connect to a configured WiFi hotspot, it instantiates its
 * own hotspot so that a user can connect to it and issue details about the 
 * local WiFi network. This class controls those attributes of the WiFi Access
 * Point server.
\*============================================================================*/
#include "wifiAPServer.h"
#include "Arduino.h"
#include "SPIFFS.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "config.h"
#include "httpHelpers.h"


/**
 * Constructor
 */
WifiAPServer::WifiAPServer(){}


/**
 * Initialise
 */
bool WifiAPServer::init(AsyncWebServer *webServer) {
  #ifdef SERIAL_DEBUG
  Serial.println(" % Initialising WiFi in Access Point mode...");
  #endif

  // Keep a pointer to the AsyncWebServer
  _webServer = webServer;

  // Make a note of the SSID configured in the config.h
  _apSSID = AP_SSID;

  WiFi.mode(WIFI_AP_STA);
  //  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
  WiFi.softAP(_apSSID); // No password makes it an open network
  _IP = WiFi.softAPIP();

  // Some default headers for handling CORS problems in javascript
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  
  // Setup the routes
  initRoutes();

  // Start the Web Server
  _webServer->begin();
 
  #ifdef SERIAL_DEBUG
  Serial.print(" % Listening on AP ");
  Serial.println(_apSSID);
  Serial.print(" % IP address: ");
  Serial.println(_IP);
  Serial.println(" % WiFi in Access Point mode initialised.");
  #endif

  return true;
}


/**
 * Used when serving HTML files to replace key variables in the HTML with
 * current state data.
 */
String WifiAPServer::templateProcessor(const String& var){
  Serial.println(var);
//  if(var == "STATE"){
//    if(digitalRead(ledPin)){
//      ledState = "ON";
//    }
//    else{
//      ledState = "OFF";
//    }
//    Serial.print(ledState);
//    return ledState;
//  }
  return String();
}


/**
 * Initialise the routes for Access Point wifi server mode
 */
void WifiAPServer::initRoutes() {
  // Route for root / web page (apmode.html)
  _webServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/apmode.html", String(), false, templateProcessor);
  });

  // All other Files / Routes
  _webServer->onNotFound([](AsyncWebServerRequest *request){
    // Attempt to load the file from the SPIFFS file system
    if ((request->method() == HTTP_GET) && SPIFFS.exists(request->url())) {
      request->send(SPIFFS, request->url(), getMimeType(request->url()));
    } 

    // Handle HTTP_OPTIONS
    else if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    }

    // Return a 404 if the file wasn't found
    else {
      #ifdef SERIAL_DEBUG
      Serial.print("Unknown Resource: ");
      Serial.print(request->url());
      Serial.print(" method: ");
      Serial.println(request->methodToString());
      #endif
      request->send(404, "text/plain", "The content you are looking for was not found.");
    }
  });
}
