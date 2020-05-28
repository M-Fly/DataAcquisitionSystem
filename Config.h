#ifndef __MFLY_DAS_CONFIG__
#define __MFLY_DAS_CONFIG__

// File to configure standard parameters for the MX-2 DAS System

// Data Transmission Information
const int TRANSMISSION_FREQUENCY_HERTZ = 5;
const char AIRCRAFT_ID[] = "MX";

// Pins for Input/Output
const int ANALOG_PIN = 0;
const int RECEIVER_PIN = 2; // Receiver Pin for internal Paylaods - Channel 3 on Futaba reciever - up on left joystick (bottom is less than 1200)
const int MODE_PIN = 12;

const int SERVO_PIN_1 = 4;
const int SERVO_PIN_2 = 5;
const int SERVO_START = 30;
const int SERVO_END = 120;

// CDA 2018
const int SERVO_PIN_CDA3 = 8;
const int SERVO_PIN_CDA2 = 9;
const int SERVO_PIN_CDA = 10; 
const int RECEIVER_PIN_CDA = 3; // Receiver Pin for CDA - Channel 2 on Futaba reciever - Up on right joystick (middle is less than 1500)
const int SERVO_START_CDA = 60;
const int SERVO_END_CDA = 150; 



// LED Setup
const int LED_PIN = 13;

// Alt Baro Setup
const int BARO_NUM_READINGS = 5;

#endif
