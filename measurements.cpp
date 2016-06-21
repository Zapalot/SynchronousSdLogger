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


#include "measurements.h"
#include "TimeKeeper.h"
#include "GpsUtils.h"
CircularBuffer<MeasRecord, measBufferSize> measBuffer;
//called from inside the timer interrupt
void measure() {
  unsigned long ticks = TimeKeeper::getTimerTicksSinceClockTick();
  //measure every fourth tick..
  /*
      unsigned long ticksSinceClock;
    unsigned long clockTicksSiceStart;
    char lastGpsTimeStamp[2  + 2  + 2 + 1 + 2 + 2 + 2  + 1]; // year, month, day, hour, minute, second
    int measurementData;
  */
  if (ticks % 4 == 0) {
    MeasRecord newRecord;
    newRecord.ticksSinceClock = ticks >> 2;
    newRecord.clockTicksSiceStart = TimeKeeper::getClockTicksSinceStart();
    memcpy(newRecord.lastGpsTimeStamp, lastGpsTimeStamp, sizeof(lastGpsTimeStamp));
    for (int i = 0; i < nMeasurements; i++) {
      newRecord.measurementData[i] = analogRead(A0 + i);
    //  Serial.println(newRecord.measurementData[i]);
    }
    measBuffer.push(newRecord);

  }
}

void MeasRecord::writeToBuffer(char* buffer) {
  int bytesTaken = sprintf(buffer, "\t%u", this->clockTicksSiceStart); // seems to work with one argument only for some weird reason.
  buffer += bytesTaken;

  bytesTaken=sprintf(buffer, "\t%u", this->ticksSinceClock); // seems to work with one argument only for some weird reason.
  buffer += bytesTaken;

  for (int i = 0; i < nMeasurements; i++) {
    bytesTaken = sprintf(buffer, "\t%u", measurementData[i]); // add argument
    buffer += bytesTaken;
  }

  bytesTaken = sprintf(buffer, "\n"); 
  buffer += bytesTaken;

  /*
    unsigned long ticksSinceClock;
    unsigned long clockTicksSiceStart;
    char lastGpsTimeStamp[2  + 2  + 2 + 1 + 2 + 2 + 2  + 1]; // year, month, day, hour, minute, second
    int measurementData;
    void writeToBuffer(char* buffer);
  */


}
