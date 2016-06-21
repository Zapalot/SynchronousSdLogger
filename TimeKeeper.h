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


#pragma once
#include "Arduino.h"
#include "measurements.h"
//#define TIME_PRESERVE_RATE  // try to keep tick rate constant rather than make up for past mistake
//#define TIME_PRESERVE_BALANCE  // try to keep tick amounts synchronous
void timerTickISR();     //timer interrupt function
void clockPulseISR(); //input in pinterrupt for GPS /PPS signal. 

class TimeKeeper {
  public:
   static void setup(unsigned long samplesPerSecond, int inputPin);
   static void internalTimerTick();
   static void externalClockTick();
    //static long getMillisSinceStart(); // adjusted for clock drift
    //static unsigned long getTimerTicksSinceStart(); 
    
   static unsigned long getTimerTicksSinceClockTick();
   static unsigned long getClockTicksSinceStart();
        
  private:
   volatile static unsigned long targetTimerTicksPerSecond; // 

   
   volatile static unsigned long timerTicksSinceClockTick;
   //volatile static unsigned long secondCountEstimate;
   volatile static unsigned long clockPulseCount;

   
   volatile static long cumulatedTimerTickBalance; //  how many Ticks in front/back are we related to the outer clock -> the algo will try to bring that to zero in the next round
   
   
   volatile static unsigned long curTimerInterval; // adjusted to make up cumulated tick difference during next clock interval
   volatile static unsigned long maxTimerInterval; // safeguard to prevent instability and oscillations
   volatile static unsigned long minTimerInterval; // safeguard to prevent instability and oscillations
    
   volatile static bool clockHasTickedFlag;
   volatile static bool timerIsStartetFlag;
      
   static void reportTimerUpdate(unsigned long nClockIntervals, unsigned long nTimerTicks);
};



//////////////////////Timer stuff:
void setTimerOnePeriod(const unsigned long  cycles);
void initializeTimerOne(const unsigned long  cycles);
