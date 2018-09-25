#ifndef __MFLY_DAS_CONFIG__
#define __MFLY_DAS_CONFIG__

// File to configure standard parameters for the MX-2 DAS System

// Data Transmission Information
const int TRANSMISSION_FREQUENCY_HERTZ = 5;
const char AIRCRAFT_ID[] = "MX";

// Pins for Input/Output
const int ANALOG_PIN = 0;
const int RECEIVER_PIN = 2;
const int MODE_PIN = 12;

const int SERVO_PIN_1 = 3;
const int SERVO_PIN_2 = 4;
const int SERVO_START = 30;
const int SERVO_END = 120;


// LED Setup
const int LED_PIN = 13;

// Alt Baro Setup
const int BARO_NUM_READINGS = 5;

#endif

