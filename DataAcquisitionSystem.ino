//M-Fly Flight Control Software

// Look for items labeled 'TODO:' for items with questions or future work

// NOTE: Altitude Measurement in Meters

#include <Servo.h>
#include <Wire.h>

#include <XBee.h>
#include <TinyGPS++.h>

#include "data.h"

// Data Collection

Data *data;
TinyGPSPlus gps;

// Constants

const int HERTZ = 5;

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

// XBee Setup

//XBee xbee;
//XBeeAddress64 broadcast = XBeeAddress64(0x00000000, 0x0000ffff);

void setup() {  
  // Initiate Serial Port
  Serial.begin(57600);
  Serial.println("Starting");

  Serial1.begin(38400);
  
  //xbee.begin(Serial);
  
  // Create Data class instance
  data = new Data();
  data->update();

  // Initiate Servos
  
  pinMode(ledPin, OUTPUT);

  Serial.println("Started");
}

void loop() {
  // Wait 1000 milliseconds to ensure no false
  // readings from the receiver 
  if (millis() < (long)1000) return;
  
  static long lastLoopTime = 0;
  static int lastAlt = 0;

  static byte lastLedState = 0;
  
  data->update();

  while (Serial1.available()) {
    int c = Serial1.read();
    
    if (gps.encode(c)) {
      Serial.print("Lat: ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(", Lon: ");
      Serial.print(gps.location.lng(), 6);
      Serial.print(", Alt: ");
      Serial.print(gps.altitude.meters(), 2);
      Serial.print(", Sats: ");
      Serial.println(gps.satellites.value());
    }
  }

  delay(20);
  //unsigned long duration = pulseIn(11, HIGH);
  
  // Blink LED and Write Data to Serial regularly
  if (millis() - lastLoopTime > DELAY_TIME) {
    lastLoopTime = millis();
    lastLedState = !lastLedState;
    
    digitalWrite(ledPin, lastLedState);
    
    //writeData();
  }
}

void writeData() {
  static char csvBuffer[128];
  
  float time = millis() / 1000.0f;

  // Drop Time and Drop Alt will both be -1 if no payload has been dropped yet.
  // A,MX2,MILLIS,ALT_BARO,AIRSPEED,DROP_TIME,DROP_ALT
  // B,MX2,MILLIS,GPS_TIME,LAT,LON,GPS_SPEED,GPS_COURSE,GPS_ALT,FIX_TIME
  // C,MX2,MILLIS,GYROX,GYROY,GYROZ,ACCELX,ACCELY,ACCELZ
  
  String message = "MX2,";
  message += time;
  message += ',';
  message += data->getAltitude();
  message += ',';
  message += data->getGyroX();
  message += ',';
  message += data->getGyroY();
  message += ',';
  message += data->getGyroZ();
  message += ',';
  message += data->getAccelX();
  message += ',';
  message += data->getAccelY();
  message += ',';
  message += data->getAccelZ();
  
  message.toCharArray(csvBuffer, message.length());

  Serial.println(message);
}

