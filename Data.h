#ifndef __MFLY_DATA
#define __MFLY_DATA

#include <arduino.h>

#include <Adafruit_Sensor.h>

#include <SFE_BMP180.h>
#include <Adafruit_BMP085_U.h>

#include <Adafruit_LSM303_U.h> //Accel/Magneto sensor library
#include <Adafruit_L3GD20_U.h> //Gyroscope library

class Data {
public:

  Data();

  float getAltitude();

  float getAccelX();
  float getAccelY();
  float getAccelZ();

  float getGyroX();
  float getGyroY();
  float getGyroZ();

  float getMagX();
  float getMagY();
  float getMagZ();
  void setBaseline();
  void update();

private:
  float getPressure();
  void setAltitude(float inputAlt);
  void setAccelX(float inputAccelX);
  void setAccelY(float inputAccelY);
  void setAccelZ(float inputAccelZ);
  void setGyroX(float inputGyroX);
  void setGyroY(float inputGyroY);
  void setGyroZ(float inputGyroZ);
  void setMagX(float inputMagX);
  void setMagY(float inputMagY);
  void setMagZ(float inputMagZ);

  void setPressure(float inputPressure);
  sensor_t mySensor;
  float accelX, accelY, accelZ;
  float gyroX,  gyroY,  gyroZ;
  float magX,   magY,   magZ;

  SFE_BMP180 pressure;

  Adafruit_LSM303_Accel_Unified accel;
  Adafruit_L3GD20_Unified gyro;
  Adafruit_LSM303_Mag_Unified mag;

  float baseline, pressure1, altitude;
};

#endif
