#pragma once
#include <Arduino.h>
#include <Adafruit_GPS.h>


// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  false
typedef char GpsTimeStampString[2  + 2  + 2 + 1 + 2 + 2 + 2  + 1];

extern GpsTimeStampString lastGpsTimeStamp; // year, month, day, hour, minute, second
void setupGPS();
void updateTimeStamp() ; //prototype
void readGPSData();

// write a number into a buffer
char* putNumber(int number, char* buffer);
// write a number into a buffer
char* putChar(char character, char* buffer);


void updateTimeStamp();

