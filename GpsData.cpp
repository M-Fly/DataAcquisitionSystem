// Code written by Ian O'Rourke (ianor@umich.edu)

#include "GpsData.h"

GpsData::GpsData() {
  // Does Nothing
}

bool GpsData::encode(int c) {
  if (gps.encode(c)) {
    // Find position, velocity, course, and altitude
    gps.get_position(&lat, &lon, &fix_age);
    speed = gps.speed();
    course = gps.course();
    altitude = gps.altitude();
    
    // Get GPS time
    gps.get_datetime(&date, &time, &fix_age);

    // Get GPS accuracy information
    hdop = gps.hdop();
    satellites = gps.satellites();

    return true;
  }

  return false;
}

