#ifndef __DAS_GPS_DATA__
#define __DAS_GPS_DATA__

#include <MicroNMEA.h>
#include <arduino.h>

class GpsData {
public:
  GpsData();

  // encode
  // REQUIRES: Valid NMEA character from serial port
  // MODIFIES: Nothing
  // EFFECTS:  Returns true or false if a complete and valid NMEA
  //            has been parsed
  bool encode(char c);

  // getLatitude
  // Returns the latitude in millionths of a degree, positive North
  long getLatitude() const;

  // getLongitude
  // Returns the longitude in millionths of a degree, positive East
  long getLongitude() const;

  // getSpeedKts
  // Returns speed in 
  long getSpeedKts() const;

  // getCourse
  // Returns the course in thousandths of a degree
  long getCourse() const;

  // getAltitudeMM
  // Returns altitude in Millimeters
  long getAltitudeMM() const;

  // isAltitudeValid
  // Returns true if most recent altitude came from a full 3D GPS lock
  bool isAltitudeValid() const;

  // getNumSatellites
  // Returns the number of satellites currently connected
  int getNumSatellites() const;

  // getHDOP
  // Returns the HDOP precision in tenths
  int getHDOP() const;
  
  // getGpsSystem
  // Returns the system the GPS is currently connected to
  //  P = GPS Only
  //  L = GLONASS Only
  //  A = Galileo Only
  //  N = GNSS -> Multiple Constellations
  //  \0 = No Fix
  char getGpsSystem() const;
  
private:
  char system;
  bool altitudeValid;
  
  long lat, lng;
  long speed;
  long course;
  long altitude;

  int satellites;
  int hdop;

  char gps_buffer[100];
  MicroNMEA nmea;
};

#endif

