#ifndef __DAS_GPS_DATA__
#define __DAS_GPS_DATA__

#include <MicroNMEA.h>
#include <arduino.h>

class GpsData {
public:
  GpsData();
  
  bool encode(char c);
  
  long getLatitude();
  long getLongitude();
  long getSpeed();
  long getCourse();
  
  long getAltitude();
  bool isAltitudeValid();

  int getNumSatellites();
  int getHDOP();

  long getLastFix();
  
private:
  char system;
  bool altitudeValid;
  
  long lat, lng;
  long speed;
  long course;
  long altitude;

  int satellites;
  int hdop;

  long lastFixMillis;

  char gps_buffer[100];
  MicroNMEA nmea;
};

#endif
