// GPS-Synchronous Data Logger

// Copyright 2016, Felix Bonowski

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include <SPI.h>
#include <Adafruit_GPS.h>
#include <SD.h>
#include <avr/sleep.h>
#include "adaUtilities.h"
#include "SdLogging.h"
#include "GpsUtils.h"
#include "TimeKeeper.h"

bool isLogging = false;

void setup() {
  // for Leonardos, if you want to debug SD issues, uncomment this line
  // to see serial output
  while (!Serial);
  Serial.begin(115200);
  Serial.println(F("\rlogger "));
  pinMode(ledPin, OUTPUT);
  inititializeSDFile();
  setupGPS();
  TimeKeeper::setup(1000, 2); // start synchonous sampling of data
  Serial.println(F("Ready! - press 'l' to start logging, 's' to stop."));


}


void loop() {
  readGPSData();

  if (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case 'l':
        //inititializeSDFile();
        isLogging = true;
        Serial.println(F("started logging!"));
        break;
      case 's':
        isLogging = false;
        logfile.flush();
        logfile.close();

        Serial.println(F("stopped logging.you can now remove the card"));
    }

  }
  if (isLogging) {

    // check out sampled data
    uint8_t remaining = 0;
    MeasRecord measResult;

    // check if data is to be written
    noInterrupts();
    remaining = measBuffer.remain();
    interrupts();
    // only write once there is a bit of data
    if (remaining > 8) {
      for (int i = 0; i < 8; i++) {
        // get one dataset
        noInterrupts();
        if (remaining) measResult = measBuffer.pop();
        interrupts();


        char buffer[50];
        measResult.writeToBuffer(buffer);
        // Serial.println(measResult.measurementData[0]);
        Serial.print(measResult.lastGpsTimeStamp);
        Serial.println(buffer);

        //write log

        logfile.write(measResult.lastGpsTimeStamp);
        logfile.write(buffer);

        // get new dataset.
        noInterrupts();
        remaining = measBuffer.remain();
        interrupts();
        logfile.flush();

      }
    }
  }

}

