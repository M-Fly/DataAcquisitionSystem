#include "GpsData.h"

GpsData::GpsData() : nmea(gps_buffer, sizeof(gps_buffer)) {
  // Initializer list sets up NMEA object above

  // Sets following parameters to indicate that the GPS does not have a lock
  system = '\0';
  altitudeValid = false;
}

bool GpsData::encode(char c) {
  // Encodes the most recent character into the NMEA object
  bool success = nmea.process(c);

  if (success && nmea.isValid()) {
    // If a new NMEA sentence has been parsed and is valid
    
    // Set all the local variables to those present in the most recent GPS datapacket
    system = nmea.getNavSystem();

    lat = nmea.getLatitude();
    lng = nmea.getLongitude();

    speed = nmea.getSpeed();
    course = nmea.getCourse();

    altitudeValid = nmea.getAltitude(altitude);

    satellites = nmea.getNumSatellites();
    hdop = nmea.getHDOP();

    return true;
  } else {
    // Otherwise, return false (no valid sentence parsed)
    return false;
  }
}

long GpsData::getLatitude() const {
  return lat;
}

long GpsData::getLongitude() const {
  return lng;
}

long GpsData::getSpeedKts() const {
  return speed;
}

long GpsData::getCourse() const {
  return course;
}

long GpsData::getAltitudeMM() const {
  return altitude;
}

bool GpsData::isAltitudeValid() const {
  return altitudeValid;
}

int GpsData::getNumSatellites() const {
  return satellites;
}

int GpsData::getHDOP() const {
  return hdop;
}

char GpsData::getGpsSystem() const {
  return system;
}
