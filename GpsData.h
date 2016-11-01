#ifndef __GPS_DATA__
#define __GPS_DATA__

// Code written by Ian O'Rourke (ianor@umich.edu)

#include <TinyGPS.h>

class GpsData {
public:
  GpsData();
  
  bool encode(int c);

  long lat;
  long lon;
  long course;
  long speed;
  long altitude;

  // GPS Precision data
  long hdop;
  short satellites;

  // GPS Time data
  unsigned long time;
  unsigned long date;
  unsigned long fix_age;
  
private:
  // TinyGPS object to decode NMEA data
  TinyGPS gps;
};

#endif
