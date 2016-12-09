#ifndef __MFLY_DAS_CONFIG__
#define __MFLY_DAS_CONFIG__

// File to configure standard parameters for the MX-2 DAS System

// Data Transmission Information
const int TRANSMISSION_FREQUENCY_HERTZ = 5;
const char AIRCRAFT_ID[] = "MX2";

// Pins for Input/Output
const int ANALOG_PIN = 0;
const int RECEIVER_PIN = 4;
const int SERVO_PIN = 2;

// LED Setup
const int LED_PIN = 13;

#endif
