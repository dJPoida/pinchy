/*============================================================================*\
 * Pinchy - PinchyFS
 * Peter Eldred 2020-06
 * 
 * This class manages all of the file system methods required for Pinchy
\*============================================================================*/
#include "pinchyFS.h"
#include "Arduino.h"
#include "SPIFFS.h"
#include "config.h"
#include <ArduinoJson.h>


/**
 * Constructor
 */
PinchyFS::PinchyFS(){
  _writingConfig = false; 
}


/**
 * Initialise
 */
bool PinchyFS::init() {
  #ifdef SERIAL_DEBUG
  Serial.println("\nInitialising PinchyFS...\n");
  #endif

  // Start the SPI Flash Files System
  if (!SPIFFS.begin(true)) {
    #ifdef SERIAL_DEBUG
    Serial.println("! Failed to mount file system");
    #endif
    return false;
  }
  
  // Give SPIFFS a tick to catch up
  delay(250);
  
  #ifdef SERIAL_DEBUG
  Serial.println("- SPIFFS initialised.");
  #endif

  // Load the config from the onboard SPI File System
  if (!loadConfig()) {
    #ifdef SERIAL_DEBUG
    Serial.println("! Failed to load configuration from SPIFFS");
    Serial.print("- Creating new Config File... ");
    #endif

    if (!saveConfig()) {
      #ifdef SERIAL_DEBUG
      Serial.println("! Failed to create new config file on SPIFFS.");
      #endif
      
      return false;
    }
  }

  #ifdef SERIAL_DEBUG
  Serial.println("\nPinchyFS initialised.\n");
  #endif

  return true;
}


/**
 * Open up the pinchy.json file on the SPIFFS partition and store the milky goodness within
 */
bool PinchyFS::loadConfig() {
  File configFile = SPIFFS.open("/pinchy.json", "r");
  if (!configFile) {
    #ifdef SERIAL_DEBUG
    Serial.println("! Failed to open config file");
    #endif
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    #ifdef SERIAL_DEBUG
    Serial.println("! Error - config file size is too large");
    #endif
    return false;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<1024> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, configFile);

  if (error) {
    #ifdef SERIAL_DEBUG
    Serial.println("! Failed to read file, using default configuration: ");
    #endif
  } else {
    #ifdef SERIAL_DEBUG
    Serial.println("- Config loaded from 'SPIFFS/pinchy.json': ");
    #endif
  }

  // Update the global variables from the json doc
  config.wifi_ssid = doc["wifi_ssid"] | config.wifi_ssid;
  config.wifi_password = doc["wifi_pw"] | config.wifi_password;

  configFile.close();
    
  #ifdef SERIAL_DEBUG
  Serial.println("=== CONFIGURATION ===");
  Serial.print("- WiFi SSID: ");
  Serial.println(config.wifi_ssid);
  Serial.print("- WiFi Password: ");
  Serial.println(config.wifi_password);
  Serial.println("=====================");
  #endif
    
  return true;
}


/**
 * Save the current configuration to SPIFFS
 */
bool PinchyFS::saveConfig() {
  // Wait for any current write operations to finish
  while (_writingConfig) {
    delay(1);
  };
  _writingConfig = true;
  
  #ifdef SERIAL_DEBUG
  Serial.println("- Writing configuration to 'SPIFFS/pinchy.json'...");
  #endif

  // Open file for writing
  File configFile = SPIFFS.open("/pinchy.json", "w");
  if (!configFile) {
    #ifdef SERIAL_DEBUG
    Serial.println("! Failed to create 'SPIFFS/pinchy.json'!");
    #endif
    return false;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<1024> doc;

  // Set the values in the document
  doc["wifi_ssid"]  = config.wifi_ssid;
  doc["wifi_pw"]    = config.wifi_password;
  doc["ip_address"] = config.ip_address;

  // Serialize JSON to file
  if (serializeJson(doc, configFile) == 0) {
    #ifdef SERIAL_DEBUG
    Serial.println("! Failed to write to 'SPIFFS/pinchy.json'");
    #endif
  }

  // Close the file
  configFile.close();

  _writingConfig = false;
  
  return true;
}
