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


/**
 * Constructor
 */
PinchyFS::PinchyFS(){}


/**
 * Initialise
 */
bool PinchyFS::init() {
  #ifdef SERIAL_DEBUG
  Serial.println(" % Initialising PinchyFS...");
  #endif

  // Start the SPI Flash Files System
  if (!SPIFFS.begin(true)) {
    #ifdef SERIAL_DEBUG
    Serial.println("Failed to mount file system");
    #endif
    return false;
  }
  
  // Give SPIFFS a tick to catch up
  delay(250);
  
  #ifdef SERIAL_DEBUG
  Serial.println(" %% SPIFFS initialised.");
  #endif

  #ifdef SERIAL_DEBUG
  Serial.println(" % PinchyFS initialised.");
  #endif

  return true;
}
