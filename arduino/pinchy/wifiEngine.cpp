/*============================================================================*\
 * Pinchy - WifiClient
 * Peter Eldred 2020-06
 * 
 * When Pinchy is configured to connect to a WiFi hotspot, it connects as a 
 * client. This unit is initialised when pinchy is in WiFi client mode.
\*============================================================================*/
#include "wifiEngine.h"
#include "Arduino.h"
#include <DNSServer.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "config.h"
#include "httpHelpers.h"
#include "wifiCaptivePortalHandler.h"



/**
 * Constructor
 */
WiFiEngine::WiFiEngine(){}


/**
 * Initialise
 */
bool WiFiEngine::init(AsyncWebServer *webServer, DNSServer *dnsServer) {
  #ifdef SERIAL_DEBUG
  Serial.println("\nInitialising WiFi engine...\n");
  #endif

  // Keep a pointer to the AsyncWebServer and DNS Server
  _webServer = webServer;
  _dnsServer = dnsServer;

  // At this point we can consider ourselves uninitialised
  wifiEngineMode = WEM_UNINIT;
  
  // attempt to connect to the configured wifi 
  if (!connectToHotSpot()) {
    // Attempt to serve up the dedicated Access Point
    if (!broadcastAP()) {
      // Don't continue if unable to perform either connection
      return false;
    }
  }

  // Some default headers for handling CORS problems in javascript
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  
  // Setup the routes
  initRoutes();

  // Assign the WebRequestHandler
  _webServer->addHandler(new WiFiCaptivePortalHandler()).setFilter(ON_AP_FILTER);

  // Start the Web Server
  _webServer->begin();

  #ifdef SERIAL_DEBUG
  Serial.println("\nWiFi engine initialised.\n");
  #endif

  return true;
}


/**
 * Attempt to connect to the configured WiFi hotspot
 */
bool WiFiEngine::connectToHotSpot() {
  if (WiFi.status() != WL_CONNECTED) {
    // Don't bother if we don't have a configuration yet
    if (!config.wifi_ssid.equals("")) {
      #ifdef SERIAL_DEBUG
      Serial.print("- Connecting to: "); 
      Serial.println(config.wifi_ssid);
      #endif

      // Attempt to connect to the configured wifi hotspot
      WiFi.begin(config.wifi_ssid.c_str(), config.wifi_password.c_str()); 

      // Wait for the connect result
      if (WiFi.waitForConnectResult() != WL_CONNECTED) {
          #ifdef SERIAL_DEBUG
          Serial.println("! Failed to connect!");
          #endif
          return false;
      }       

      wifiEngineMode = WEM_CLIENT;
      ipAddress = WiFi.localIP().toString();
      macAddress = WiFi.macAddress();
      config.ip_address = ipAddress;
    
      #ifdef SERIAL_DEBUG
      Serial.print("- WiFi connected."); 
      Serial.print("- IP address: "); 
      Serial.println(ipAddress);
      Serial.print("- MAC address: ");
      Serial.println(macAddress);
      #endif

      return true;
    }
  }

  return false;
}


/**
 * Attempt to broadcast a dedicated Access Point
 */
bool WiFiEngine::broadcastAP() {
  #ifdef SERIAL_DEBUG
  Serial.println("- Attempting to Broadcast dedicated Access Point");
  #endif

  WiFi.mode(WIFI_AP_STA);
  
  if (!WiFi.softAP(AP_SSID)) {
    // The adapter was not able to set the desired SSID for the acces point
    #ifdef SERIAL_DEBUG
    Serial.println("! Failed to assign the softAP SSID.");
    #endif
    return false;
  }

  // Start the DNS server for the CaptiveRequestHandler
  _dnsServer->start(53, "*", WiFi.softAPIP());

  wifiEngineMode = WEM_AP;
  ipAddress = WiFi.softAPIP().toString();
  macAddress = WiFi.softAPmacAddress();
 
  #ifdef SERIAL_DEBUG
  Serial.print("- Access Point broadcasting on SSID: '");
  Serial.print(AP_SSID);
  Serial.println("'");
  Serial.print("- IP address: ");
  Serial.println(ipAddress);
  Serial.print("- MAC address: ");
  Serial.println(macAddress);
  #endif

  return true;
}


/**
 * Used when serving HTML files to replace key variables in the HTML with
 * current state data.
 */
String WiFiEngine::templateProcessor(const String& var){
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
 * Initialise the routes for serving Pinchy's web app content
 */
void WiFiEngine::initRoutes() {
  // Route for root / web page (apmode.html)
  _webServer->on("/", HTTP_GET, [&](AsyncWebServerRequest *request){
    if (wifiEngineMode == WEM_AP) {
      request->send(SPIFFS, "/ap-mode.html", String(), false, templateProcessor);
    } else {
      request->send(SPIFFS, "/index.html", String(), false, templateProcessor);
    }
  });

  // Set the wifi access point details
  _webServer->on("/setwifi", HTTP_POST, [&](AsyncWebServerRequest *request){
    handleSetWiFi(request);
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
      Serial.print("- 404: ");
      Serial.print(request->methodToString());
      Serial.print(" '");
      Serial.print(request->url());
      Serial.println("'");
      #endif
      request->send(404, "text/plain", "The content you are looking for was not found.");
    }
  });
}



/**
 * Handles setting new WiFi connection details
 */
void WiFiEngine::handleSetWiFi(AsyncWebServerRequest *request){
//    AsyncWebParameter* newSSID = request->getParam("wifiSSID");
//    AsyncWebParameter* newPassword = request->getParam("wifiPassword");
//    
//    setWiFiSettings(newSSID->value(), newPassword->value());
//    
//    request->send(200, "text/json", "{\"success\":true}");
}
