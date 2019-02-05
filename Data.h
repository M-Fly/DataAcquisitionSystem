#ifndef __MFLY_DATA
#define __MFLY_DATA

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
// #include "Variables.h"
#include <Adafruit_BNO055.h>
#include <Adafruit_MPL3115A2.h>

class Data {
public:
  Data();
  
  float getAccelX() const;
  float getAccelY() const;
  float getAccelZ() const;

  float getGyroX() const;
  float getGyroY() const;
  float getGyroZ() const;

  float getMagX() const;
  float getMagY() const;
  float getMagZ() const;

  float getAltitude() const;
  float getPressure() const;
  float getTemperature() const;
  
  void update();

private:
  sensors_event_t magEvent;
  sensors_event_t gyroEvent;
  sensors_event_t accelEvent;
  
  sensors_vec_t orientation;
  
  float accelX, accelY, accelZ;
  float gyroX,  gyroY,  gyroZ;
  float magX,   magY,   magZ;
  float baseline, pressure, temperature, altitude;

  imu::Vector<3> accel;
  imu::Vector<3> gyros;

  // Adafruit_BMP085_Unified bmp;
  // Adafruit_LSM303_Accel_Unified accel;
  // Adafruit_L3GD20_Unified gyro;
  // Adafruit_LSM303_Mag_Unified mag;
  
  Adafruit_BNO055 bno/*  = Adafruit_BNO055() */;
  Adafruit_MPL3115A2 baro/*  = Adafruit_MPL3115A2() */;
};

#endif


