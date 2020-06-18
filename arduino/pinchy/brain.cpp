/*============================================================================*\
 * Pinchy - Brain
 * Peter Eldred 2020-06
 * 
 * This class is the main control unit which interprets inputs and coordinates
 * outputs and other control units. Just like a hooman!
\*============================================================================*/
#include "brain.h"
#include "Arduino.h"
#include "config.h"


/**
 * Constructor
 */
Brain::Brain() {};


/**
 * Initialise
 */
void Brain::init() {
  #ifdef SERIAL_DEBUG
  Serial.println(" % Initialising brain...");
  #endif



  #ifdef SERIAL_DEBUG
  Serial.println(" % Brain initialised.");
  #endif
}
