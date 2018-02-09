//     __  ___      ________         _________       __    __     ______            __             __   _____       ______                         
//    /  |/  /     / ____/ /_  __   / ____/ (_)___ _/ /_  / /_   / ____/___  ____  / /__________  / /  / ___/____  / __/ /__      ______ _________ 
//   / /|_/ /_____/ /_  / / / / /  / /_  / / / __ `/ __ \/ __/  / /   / __ \/ __ \/ __/ ___/ __ \/ /   \__ \/ __ \/ /_/ __/ | /| / / __ `/ ___/ _ \
//  / /  / /_____/ __/ / / /_/ /  / __/ / / / /_/ / / / / /_   / /___/ /_/ / / / / /_/ /  / /_/ / /   ___/ / /_/ / __/ /_ | |/ |/ / /_/ / /  /  __/
// /_/  /_/     /_/   /_/\__, /  /_/   /_/_/\__, /_/ /_/\__/   \____/\____/_/ /_/\__/_/   \____/_/   /____/\____/_/  \__/ |__/|__/\__,_/_/   \___/ 
//                      /____/             /____/                                                                                                  
//

#include <Servo.h>
#include <Wire.h>

#include "Data.h"
#include "GpsData.h"

#include <Adafruit_BNO055.h>
#include <Adafruit_MPL3115A2.h>

#include "Config.h"

// Data Collection

Data *data = 0;
GpsData gps;
Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
bool DMESSAGE = true;

// Serial Configuration

const HardwareSerial *gpsSerial = &Serial1;
const HardwareSerial *xbeeSerial = &Serial3;

// Airspeed Data

int airspeed = 0;

// Drop System

Servo dropServo_1;
Servo dropServo_2;

int dropAlt = -1;
long dropTime = -1;

// Time Constants for Data Collection
const int DELAY_TIME = 1000 / TRANSMISSION_FREQUENCY_HERTZ; // For Loop

enum MessageType {
  StandardMessage = 0,
  GpsMessage = 1,
  GyroAccelMessage = 2,
  DMessage = 3
};

float base_alt = 0;

// Setup Function

void setup() {
  delay(500);
  
  // Initiate USB Serial Port
  Serial.begin(57600);
  Serial.println("Starting");

  // GPS Serial Port
  gpsSerial->begin(38400);

  // XBee Serial Port
  xbeeSerial->begin(38400);

  data = new Data();

  // Initiate Servos
  pinMode(RECEIVER_PIN, INPUT);
  
  dropServo_1.attach(SERVO_PIN_1);
  dropServo_1.write(SERVO_START);

  dropServo_2.attach(SERVO_PIN_2);
  dropServo_2.write(SERVO_START);

  //Set up bno
  if(!bno.begin())
  {
    Serial.print("No BNO055");
  }

  //Set up baro
  if (!baro.begin())
  {
    Serial.println("No MPL3115A2");
    DMESSAGE = false;
    return;
  }
  
  bno.setExtCrystalUse(true);

  // Final Steps
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Started");

  int i;
  for(i = 0; i < BARO_NUM_READINGS; ++i)
  {
    base_alt += baro.getAltitude();
//    Serial.print(i);
//    Serial.print(" ");
//    Serial.print(base_alt);
//    Serial.println("");
  }
  base_alt /= i;
//  Serial.println(base_alt);
}

void loop() {
  static long lastLoopTime = 0;
  static long lastLoopTime_2 = 0;
  static byte lastLedState = 0;

  static bool newGPSData = false;
  
  // Wait 1000 milliseconds to ensure no false
  // readings from the receiver 
  if (millis() < (long)1000) return;

  while (gpsSerial->available()) {
    char c = gpsSerial->read();
    
    if (gps.encode(c)) {
      newGPSData = true;
    }
  }

  // Read in serial pulse from receiver
  
  // WHEN RECEIVER ISN'T PLUGGED IN, UNCOMMENT THIS LINE AND SET dropPulse to 0!
  // Otherwise, this may cause problems with the GPS
  
  long dropPulse = pulseIn(RECEIVER_PIN, HIGH);
//  long dropPulse = 0;
    
  if (dropPulse < 1000) {
    dropServo_1.write(SERVO_END);
    dropServo_2.write(SERVO_END);
    
    dropTime = millis();
    dropAlt = data->getAltitude();
  }
  else {
    dropServo_1.write(SERVO_START);
    dropServo_2.write(SERVO_START);
  }
  
  // Blink LED and Write Data to Serial regularly
  if (millis() - lastLoopTime > DELAY_TIME) {
    // Update data object and get new airspeed analog value
    data->update();
    airspeed = analogRead(ANALOG_PIN);

    // Set the new LED state
    lastLoopTime = millis();
    lastLedState = !lastLedState;
    digitalWrite(LED_PIN, lastLedState);

    // Write GPS data if new data exists
    if (newGPSData) {
      writeData(GpsMessage);
      newGPSData = false;
    }

    // Write other messages for user
    writeData(StandardMessage);
    writeData(GyroAccelMessage);    
  }
  
  if(millis() - lastLoopTime_2 >= 1000)
  {
    writeData(DMessage);
    lastLoopTime_2 = millis();
  }
}

void writeData(MessageType m) {
  // TODO: Replace all message += with Serial2.print, no need to print to usb debugging for final version

  String message = "";

  const static char DELIN = ',';
  const static char ENDL = ';';
  
  if (m == StandardMessage) {

    // A,MX2,MILLIS,ALT_BARO,ANALOG_PITOT,PRESS,TEMP,DROP_TIME,DROP_ALT
    // Drop time and altitude will be -1 until drop.
    
    message += "A,";
    message += AIRCRAFT_ID;
    message += DELIN;
    message += millis();
    message += DELIN;
    message += data->getAltitude();
    message += DELIN;
    message += airspeed;
    message += DELIN;
    message += data->getPressure();
    message += DELIN;
    message += data->getTemperature();
    message += DELIN;
    message += dropTime;
    message += DELIN;
    message += dropAlt;
    
  } else if (m == GpsMessage) {

    // B,MX2,MILLIS,GPS_SYSTEM,LAT,LON,GPS_SPEED,GPS_COURSE,GPS_ALT,GPS_HDOP
    
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
    
  } else if (m == GyroAccelMessage) {

    // C,MX2,MILLIS,GYROX,GYROY,GYROZ,ACCELX,ACCELY,ACCELZ
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyros = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    
    message += "C,";
    message += AIRCRAFT_ID;
    message += DELIN;
    message += millis();
    message += DELIN;
    message += gyros.x();
    message += DELIN;
    message += gyros.y();
    message += DELIN;
    message += gyros.z();
    message += DELIN;
    message += accel.x();
    message += DELIN;
    message += accel.y();
    message += DELIN;
    message += accel.z();
  } else if (m == DMessage) {
    // D,MX,MILLIS,BAROALT

    message += "D,";
    message += AIRCRAFT_ID;
    message += DELIN;
    message += millis();
    message += DELIN;
    message += (baro.getAltitude() - base_alt);
  }

  message += ENDL;
  
  Serial.println(message);
  xbeeSerial->print(message);
}

