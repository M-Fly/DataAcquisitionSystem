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
#include "Variables.h"
#include "HardwareSerial.h"
#include <Adafruit_BNO055.h>
#include <Adafruit_MPL3115A2.h>

#include "Config.h"

// Data Collection

Data *data = 0;
GpsData gps;
Adafruit_BNO055 bno = Adafruit_BNO055();             // Creates Barometer and IMU objects!
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
bool DMESSAGE = true;
bool FLIGHTMODE = false;

// Serial Configuration

const HardwareSerial *gpsSerial = &Serial1;
const HardwareSerial *xbeeSerial = &Serial3;

// Airspeed Data

int airspeed = 0;

// Drop System

Servo dropServo_1;
Servo dropServo_2;
Servo dropServo_CDA;
Servo dropServo_CDA2; 
Servo dropServo_CDA3; 

int dropAlt = -1;
long dropTime = -1;
// CDA drop variables
int dropAlt_CDA = -1; 
long dropTime_CDA = -1; 

// Time Constants for Data Collection
const int DELAY_TIME = 1000 / TRANSMISSION_FREQUENCY_HERTZ; // For Loop

enum MessageType {
  StandardMessage = 0,
  GpsMessage = 1,
  GyroAccelMessage = 2,
  DMessage = 3
};

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

 // data = new Data();

  // Initiate Servos
  pinMode(RECEIVER_PIN, INPUT);
  //pinMode(MODE_PIN, INPUT);
  pinMode(RECEIVER_PIN_CDA, INPUT);
  
  dropServo_1.attach(SERVO_PIN_1);
  dropServo_1.write(SERVO_START);

  dropServo_2.attach(SERVO_PIN_2);
  dropServo_2.write(SERVO_START);

  //Set up CDA Servos
  dropServo_CDA.attach(SERVO_PIN_CDA);
  dropServo_CDA.write(SERVO_START_CDA);

  dropServo_CDA2.attach(SERVO_PIN_CDA2);
  dropServo_CDA2.write(SERVO_START_CDA);

  dropServo_CDA3.attach(SERVO_PIN_CDA3);
  dropServo_CDA3.write(SERVO_START_CDA);

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

  //Loop for setting MPL3115A2 baseline
  //Throw away first 3 readings
  for(int j = 0; j < 3; ++j)
  {
    baro.getAltitude();
  }
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
//  Serial.print("Base Alt: ");
//  Serial.print(base_alt);
//  Serial.println("");
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
 //long dropPulse = 0;  
// long dropPulse_CDA = 0; 
long dropPulse = pulseIn(RECEIVER_PIN, HIGH,25000);
long dropPulse_CDA = pulseIn(RECEIVER_PIN_CDA, HIGH,23000);
  //long modePulse = pulseIn(MODE_PIN, HIGH);

  //Serial.println(modePulse);
  //Serial.println(FLIGHTMODE);
  //Serial.println(data->getAltitude());
  //if (modePulse > 1000)
  //{
  //  FLIGHTMODE = true;
  //}

  // Internal Payloads


  if (dropPulse > 1200) {  // Channel 3 on Futaba 
    /*if ((FLIGHTMODE == true) && (data->getAltitude() > 25))
    {
      dropServo_1.write(SERVO_END);
      dropServo_2.write(SERVO_END);
    
      dropTime = millis();
      dropAlt = cur_alt;
    }*/

      dropServo_1.write(SERVO_END);
      dropServo_2.write(SERVO_END);

      dropTime = millis();
      dropAlt = cur_alt;
 
  }
  else {
    dropServo_1.write(SERVO_START);
    dropServo_2.write(SERVO_START);
  }

  // Gliders
  if (dropPulse_CDA > 1550)
  {
    dropServo_CDA.write(SERVO_END_CDA);
    dropServo_CDA2.write(SERVO_END_CDA);
    dropServo_CDA3.write(SERVO_END_CDA);
    
    dropAlt_CDA = cur_alt;
    dropTime_CDA = millis(); 
  } else
  {
    dropServo_CDA.write(SERVO_START_CDA);
    dropServo_CDA2.write(SERVO_START_CDA);
    dropServo_CDA3.write(SERVO_START_CDA);
  }
  
  // Blink LED and Write Data to Serial regularly
  if (millis() - lastLoopTime > DELAY_TIME) {
    // Update data object and get new airspeed analog value
//    data->update();
    updateBNO055();
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
    // Transmit D message
    writeData(DMessage);    
  }

  // Loop for MPL3115A2
        // MPL3115A2 does not read more than once per second without annoying code
        // Reading it at the same time as everything else causes delays
        if(millis() - lastLoopTime_2 >= 1000)
        {
          // Read in altitude, store in cur_alt
          cur_alt = (baro.getAltitude() - base_alt);
          if(cur_alt < 0)
          {
            //Serial.println(cur_alt);
            //base_alt = base_alt - (abs(cur_alt)/zero_count);
            //++zero_count;
            if(cur_alt < -1)
            {
              base_alt = base_alt - 1;
            }
            
            cur_alt = 0;
          }
          else if(data->getAltitude() < 0)
          {
            cur_alt /= 2;
            base_alt = base_alt + (cur_alt/2);
          }
      
          // Reset timer
          lastLoopTime_2 = millis();
        }
    
 }

void writeData(MessageType m) {
  // TODO: Replace all message += with Serial2.print, no need to print to usb debugging for final version

  String message = "";

  const static char DELIN = ',';
  const static char ENDL = ';';
  
  if (m == StandardMessage) {

    // A,MX2,MILLIS,ALT_BARO,ANALOG_PITOT,PRESS,TEMP,DROP_TIME,DROP_ALT,DROP_TIME_CDA, DROP_ALT_CDA
    // Drop times and altitudes will be -1 until drop.
    
    message += "A,";
    message += AIRCRAFT_ID;
    message += DELIN;
    message += millis();
    message += DELIN;
    message += cur_alt;
    message += DELIN;
    message += airspeed;
    message += DELIN;
    message += 0;
    message += DELIN;
    message += 0;
    message += DELIN;
    message += dropTime;
    message += DELIN;
    message += dropAlt;
    message += DELIN;
    message += dropTime_CDA; 
    message += DELIN;
    message += dropAlt_CDA; 
    
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
    } 
    
  else if (m == DMessage) {
    // D,MX,MILLIS,BAROALT

    message += "D,";
    message += AIRCRAFT_ID;
    message += DELIN;
    message += millis();
    message += DELIN;
    message += cur_alt;
  }

  message += ENDL;
  
  Serial.println(message);

  xbeeSerial->print(message);
}

// Modifies: accel and gyros vectors
// Effects: Updates BNO055 IMU values
void updateBNO055()
{
    accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    gyros = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
}
