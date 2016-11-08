#include "GpsData.h"

GpsData::GpsData() : nmea(gps_buffer, sizeof(gps_buffer)) {
  // Initializer list sets up NMEA object above

  // Sets following parameters to indicate that the GPS does not have a lock
  system = '\0';
  altitudeValid = false;
  lastFixMillis = 0;
}

bool GpsData::encode(char c) {
  bool success = nmea.process(c);

  if (success && nmea.isValid()) {
    system = nmea.getNavSystem();

    lat = nmea.getLatitude();
    lng = nmea.getLongitude();

    speed = nmea.getSpeed();
    course = nmea.getCourse();

    altitudeValid = nmea.getAltitude(altitude);

    satellites = nmea.getNumSatellites();
    hdop = nmea.getHDOP();

    lastFixMillis = millis();

    return true;
  } else {
    return false;
  }
}

long GpsData::getLatitude() {
  return lat;
}

long GpsData::getLongitude() {
  return lng;
}

long GpsData::getSpeed() {
  return speed;
}

long GpsData::getCourse() {
  return course;
}

long GpsData::getAltitude() {
  return altitude;
}

bool GpsData::isAltitudeValid() {
  return altitudeValid;
}

int GpsData::getNumSatellites() {
  return satellites;
}

int GpsData::getHDOP() {
  return hdop;
}

long GpsData::getLastFix() {
  return lastFixMillis;
}

