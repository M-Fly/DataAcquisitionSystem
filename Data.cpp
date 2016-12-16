/*
Code written 10/16/15
Written by Daniel and James
Function Comments written by Daniel

Updated Nov. 22, 2016 - Ian O'Rourke
*/

#include "Data.h"

Data::Data() : accel(30301), mag(30302), gyro(20), bmp(18001) {
  accel.begin();
  mag.begin();
  gyro.begin();  
  bmp.begin(BMP085_MODE_ULTRAHIGHRES);

  update();
  baseline = getPressure();
}

/*
Returns the (X/Y/Z) Value of the Accerlation
Should be called immediately after update();
Returns: AcccelX/AccelY/AccelZ as a float
*/
float Data::getAccelX() const {
  return accelX;
}

float Data::getAccelY() const {
  return accelY;
}

float Data::getAccelZ() const {
  return accelZ;
}

/*
Returns the (X/Y/Z) Value of the Gyroscope
Should be called immediately after update();
Returns: gyroX/gyroY/gyroZ as a float
*/
float Data::getGyroX() const {
  return gyroX;
}

float Data::getGyroY() const {
  return gyroY;
}

float Data::getGyroZ() const {
  return gyroZ;
}

/*
Returns the (X/Y/Z) Value of the magnetometer
Should be called immediately after update();
Returns: magX/magY/magZ as a float
*/
float Data::getMagX() const {
  return magX;
}

float Data::getMagY() const {
  return magY;
}

float Data::getMagZ() const {
  return magZ;
}

float Data::getRoll() const {
  return roll;
}

float Data::getPitch() const {
  return pitch;
}

float Data::getHeading() const {
  return heading;
}

/*
Returns the current Altitude
Should be called immediately after update();
Returns: altitude as a float
*/
float Data::getAltitude() const {
  return altitude;
}

/*
Returns the current pressure
Should be called immediately after update();
Returns: pressure as a float
*/
float Data::getPressure() const
{
  return pressure;
}

/*
Returns the current temperature
Should be called immediately after update();
Returns: temperature as a float
*/
float Data::getTemperature() const
{
  return temperature;
}

/*
updates all of the sensor data to the most recent values
calls the sensor's functions to access the values
this should always be called prior to accessing any of the values
*/
void Data::update() {
  // Get Acceleration
  accel.getEvent(&accelEvent);
  accelX = accelEvent.acceleration.x;
  accelY = accelEvent.acceleration.y;
  accelZ = accelEvent.acceleration.z;

  gyro.getEvent(&gyroEvent);
  gyroX = gyroEvent.gyro.x;
  gyroY = gyroEvent.gyro.y;
  gyroZ = gyroEvent.gyro.z;
  
  mag.getEvent(&magEvent);
  magX = magEvent.magnetic.x;
  magY = magEvent.magnetic.y;
  magZ = magEvent.magnetic.z;

 if (dof.fusionGetOrientation(&accelEvent, &magEvent, &orientation)) {
  roll = orientation.roll;
  pitch = orientation.pitch;
 }

  bmp.getPressure(&pressure);
  bmp.getTemperature(&temperature);

  altitude = bmp.pressureToAltitude(baseline, pressure);
}

