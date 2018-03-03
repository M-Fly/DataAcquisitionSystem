#ifndef __MFLY_DAS_Variables__
#define __MFLY_DAS_Variables__

#include <Adafruit_BNO055.h>

// Set up global variables

// Base Altitudes
float base_alt = 0;
float gps_base = 0;
int zero_count = 2;

// Current Altitude
float cur_alt = -1;

// Acceleration and Gyro values. Vector carries x, y, and z values
imu::Vector<3> accel;
imu::Vector<3> gyros;

// Rolling Mean variables for GyroX, GyroY, GyroZ, AccelX, AccelY, AccelZ,
const int numReadings = 4;         //Number of readings for rolling mean
int readIndex = 0;                 //Index for current reading

float GXreadings[numReadings];     //Readings array for GyroX
float GYreadings[numReadings];     //Readings array for GyroY
float GZreadings[numReadings];     //Readings array for GyroZ
float AXreadings[numReadings];     //Readings array for AccelX
float AYreadings[numReadings];     //Readings array for AccelY
float AZreadings[numReadings];     //Readings array for AccelZ

float GXtotal = 0;                 //Running total for GyroX
float GYtotal = 0;                 //Running total for GyroY
float GZtotal = 0;                 //Running total for GyroZ
float AXtotal = 0;                 //Running total for AccelX
float AYtotal = 0;                 //Running total for AccelY
float AZtotal = 0;                 //Running total for AccelZ

float GXavg = 0;                   //Average for GyroX
float GYavg = 0;                   //Average for GyroY
float GZavg = 0;                   //Average for GyroZ
float AXavg = 0;                   //Average for AccelX
float AYavg = 0;                   //Average for AccelY
float AZavg = 0;                   //Average for AccelZ

#endif

