//     __  ___      ________         _________       __    __     ______            __             __   _____       ______                         
//    /  |/  /     / ____/ /_  __   / ____/ (_)___ _/ /_  / /_   / ____/___  ____  / /__________  / /  / ___/____  / __/ /__      ______ _________ 
//   / /|_/ /_____/ /_  / / / / /  / /_  / / / __ `/ __ \/ __/  / /   / __ \/ __ \/ __/ ___/ __ \/ /   \__ \/ __ \/ /_/ __/ | /| / / __ `/ ___/ _ \
//  / /  / /_____/ __/ / / /_/ /  / __/ / / / /_/ / / / / /_   / /___/ /_/ / / / / /_/ /  / /_/ / /   ___/ / /_/ / __/ /_ | |/ |/ / /_/ / /  /  __/
// /_/  /_/     /_/   /_/\__, /  /_/   /_/_/\__, /_/ /_/\__/   \____/\____/_/ /_/\__/_/   \____/_/   /____/\____/_/  \__/ |__/|__/\__,_/_/   \___/ 
//                      /____/             /____/                                                                                                  
//
// Look for items labeled 'TODO:' for items with questions or future work
// TODO: getSystem in GpsData.h -> use enum?

// TODO: Fix Altitude 

#include <Servo.h>
#include <Wire.h>

#include "data.h"
#include "GpsData.h"

// Data Collection

Data *data;
GpsData gps;

// Constants

const int HERTZ = 5;
const char AIRCRAFT_ID[] = "MX2";

// Time Constants for Data Collection

const int DELAY_TIME = 1000 / HERTZ; // For Loop

enum MessageType {
  StandardMessage = 0,
  GPS = 1,
  GyroAccel = 2
};

// Drop Status Parameters

int dropAlt = -1;
long dropTime = -1;

// Servo Setup

// LED Setup

const int ledPin = 13;

void setup() {  
  // Initiate USB Serial Port
  Serial.begin(57600);
  Serial.println("Starting");

  // GPS Serial Port
  Serial1.begin(38400);

  // XBee Serial Port
  Serial2.begin(38400);
  
  // Create Data class instance
  data = new Data();
  data->update();

  // Initiate Servos
  
  pinMode(ledPin, OUTPUT);

  Serial.println("Started");
}

void loop() {
  static long lastLoopTime = 0;
  static int lastAlt = 0;

  static byte lastLedState = 0;
  
  // Wait 1000 milliseconds to ensure no false
  // readings from the receiver 
  if (millis() < (long)1000) return;
  
  data->update();

  while (Serial1.available()) {
    char c = Serial1.read();
    //Serial.print(c);
    
    if (gps.encode(c)) {
      Serial.print("Lat: ");
      Serial.print(gps.getLatitude());
      Serial.print(", Lon: ");
      Serial.print(gps.getLongitude());
      Serial.print(", Alt: ");
      Serial.print(gps.getAltitudeMM());
      Serial.print(", Sats: ");
      Serial.println(gps.getNumSatellites());
    }
  }
  
  // Blink LED and Write Data to Serial regularly
  if (millis() - lastLoopTime > DELAY_TIME) {
    lastLoopTime = millis();
    lastLedState = !lastLedState;
    
    digitalWrite(ledPin, lastLedState);

    if (millis() - gps.getLastFixMillis() > 1000) Serial.println("Waiting for GPS Lock");
    
    writeData(StandardMessage);
    writeData(GyroAccel);
  }
}

void writeData(MessageType m) {
  static char csvBuffer[256];

  String message = "";

  static char DELIN = ',';
  static char ENDL = ';';

  int airspeed_TESTING = 30003;
  int dropTime_TESTING = -1;
  int dropAlt_TESTING = -1;
  
  if (m == StandardMessage) {

    // A,MX2,MILLIS,ALT_BARO,AIRSPEED,DROP_TIME,DROP_ALT
    // Drop time and altitude will be -1 until drop.
    
    message += "A,";
    message += AIRCRAFT_ID;
    message += DELIN;
    message += millis();
    message += DELIN;
    message += data->getAltitude();
    message += DELIN;
    message += airspeed_TESTING;
    message += DELIN;
    message += dropTime_TESTING;
    message += DELIN;
    message += dropAlt_TESTING;
    
  } else if (m == GPS) {

    // B,MX2,MILLIS,GPS_SYSTEM,LAT,LON,GPS_SPEED,GPS_COURSE,GPS_ALT,GPS_HDOP,FIX_TIME
    
    message += "B,";
    message += AIRCRAFT_ID;
    message += DELIN;
    message += millis();
    message += DELIN;
    message += gps.getGpsSystem();
    message += DELIN;
    message += gps.getLatitude();
    message += DELIN;
    message += gps.getLongitude();
    message += DELIN;
    message += gps.getSpeedKts();
    message += DELIN;
    message += gps.getCourse();
    message += DELIN;
    message += gps.getAltitudeMM();
    message += DELIN;
    message += gps.getHDOP();
    message += DELIN;
    message += gps.getLastFixMillis();
    
  } else if (m == GyroAccel) {

    // C,MX2,MILLIS,GYROX,GYROY,GYROZ,ACCELX,ACCELY,ACCELZ
    
    message += "C,";
    message += AIRCRAFT_ID;
    message += DELIN;
    message += millis();
    message += DELIN;
    message += data->getGyroX();
    message += DELIN;
    message += data->getGyroY();
    message += DELIN;
    message += data->getGyroZ();
    message += DELIN;
    message += data->getAccelX();
    message += DELIN;
    message += data->getAccelY();
    message += DELIN;
    message += data->getAccelZ();
  }

  message += ENDL;
  
  message.toCharArray(csvBuffer, message.length());
  Serial.println(message);
  Serial2.print(message);
}

