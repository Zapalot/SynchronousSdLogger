#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "adaUtilities.h"


// Set the pins used
#define chipSelect 10
File logfile;

void inititializeSDFile() {
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
//  if (!SD.begin(chipSelect, 11, 12, 13)) {
    if (!SD.begin(chipSelect)) {      // if you're using an UNO, you can use this line instead
    Serial.println(F("Card init. failed!"));
    blinkErrorCode(2);
  }
  char filename[15];
  strcpy(filename, "GPSLOG00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i / 10;
    filename[7] = '0' + i % 10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if ( ! logfile ) {
    Serial.print("Couldnt create ");
    Serial.println(filename);
    blinkErrorCode(3);
  }
  Serial.print("Writing to ");
  Serial.println(filename);

}

