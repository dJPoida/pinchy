#include "SPIFFS.h"
#include "httpHelpers.h"

class WiFiCaptivePortalHandler : public AsyncWebHandler {
public:
  WiFiCaptivePortalHandler() {}
  virtual ~WiFiCaptivePortalHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");

    // Only return true if the request host is not that of the device
    return (request->host() != wifiEngine.ipAddress);
  }

  void handleRequest(AsyncWebServerRequest *request) {
    #ifdef SERIAL_DEBUG
    Serial.print("> Incoming request to: '");
    Serial.print(request->host().c_str());
    Serial.print("'. Redirecting to 'http://");
    Serial.print(wifiEngine.ipAddress);
    Serial.println("/ap-mode.html'...");
    #endif
  
    request->redirect("http://" + wifiEngine.ipAddress);
  }
};
