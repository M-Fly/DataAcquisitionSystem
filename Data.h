#ifndef __MFLY_DATA
#define __MFLY_DATA

#include <arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_10DOF.h>

#include <Adafruit_BMP085_U.h> // Barometer sensor library
#include <Adafruit_LSM303_U.h> // Accel/Magneto sensor library
#include <Adafruit_L3GD20_U.h> // Gyroscope sensor library

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

  int getRoll() const;
  int getPitch() const;
  int getHeading() const;

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
  int roll,   pitch,  heading;
  float baseline, pressure, temperature, altitude;

  Adafruit_10DOF dof;
  Adafruit_BMP085_Unified bmp;
  Adafruit_LSM303_Accel_Unified accel;
  Adafruit_L3GD20_Unified gyro;
  Adafruit_LSM303_Mag_Unified mag;
};

#endif

