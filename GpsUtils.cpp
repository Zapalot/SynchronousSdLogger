#include "GpsUtils.h"


GpsTimeStampString lastGpsTimeStamp; // year, month, day, hour, minute, second
//SoftwareSerial mySerial(0, 1);
Adafruit_GPS GPS(&Serial1);
// this keeps track of whether we're using the interrupt
// off by default!

void setupGPS() {
  // connect to the GPS at the desired rate
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);   // 100 millihertz (once every 10 seconds), 1Hz or 5Hz update rate

  // Turn off updates on antenna status, if the firmware permits it
  GPS.sendCommand(PGCMD_NOANTENNA);
}

void readGPSData() {
  // read data from the GPS in the 'main loop'

  char c = GPS.read();
  while (c) {
    // show debug out on serial.
    if (GPSECHO)
      if (c) Serial.print(c);
    c = GPS.read();
  }
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
   // Serial.println(F("got a line"));

    // Don't call lastNMEA more than once between parse calls!  Calling lastNMEA
    // will clear the received flag and can cause very subtle race conditions if
    // new data comes in before parse is called again.
    char *stringptr = GPS.lastNMEA();

    if (!GPS.parse(stringptr)){   // this also sets the newNMEAreceived() flag to false
    //Serial.println(F("line was broken"));
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
    updateTimeStamp();
  }
}

// write a number into a buffer
char* putNumber(int number, char* buffer) {
  int decimals[] = {10, 1};


    *buffer = '0' + number/10;    buffer++;
    *buffer = '0' + number%10;    buffer++;

  return buffer;
}
// write a number into a buffer
char* putChar(char character, char* buffer) {
  *buffer = character;
  buffer++;
  return buffer;
}


void updateTimeStamp() {
  char* buff_point = lastGpsTimeStamp;
  buff_point = putNumber(GPS.year, buff_point);

  buff_point = putNumber(GPS.month, buff_point);
  buff_point = putNumber(GPS.day, buff_point);
  buff_point = putChar('_', buff_point);
  buff_point = putNumber(GPS.hour, buff_point);
  buff_point = putNumber(GPS.minute, buff_point);
  buff_point = putNumber(GPS.seconds, buff_point);
  buff_point = putChar(0, buff_point);
}


