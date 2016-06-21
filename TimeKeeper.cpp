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


#include "TimeKeeper.h"

void measure(); //prototype

volatile  long TimeKeeper::cumulatedTimerTickBalance = 0; //  how many Ticks in front/back are we related to the outer clock -> the algo will try to bring that to zero in the next round

//volatile  unsigned long TimeKeeper::secondCountEstimate=0;
volatile  unsigned long TimeKeeper::clockPulseCount = 0;
volatile  unsigned long TimeKeeper::timerTicksSinceClockTick = 0;
volatile  bool TimeKeeper::clockHasTickedFlag = false;
volatile  bool TimeKeeper::timerIsStartetFlag = false;
volatile  unsigned long TimeKeeper::targetTimerTicksPerSecond = 0;
volatile  unsigned long TimeKeeper::curTimerInterval = 0;
volatile  unsigned long TimeKeeper::maxTimerInterval = 1000000; // safeguard to prevent instability and oscillations
volatile  unsigned long TimeKeeper::minTimerInterval = 10000; // safeguard to prevent instability and oscillations


unsigned long TimeKeeper::getTimerTicksSinceClockTick() {
  return timerTicksSinceClockTick;
}


unsigned long TimeKeeper::getClockTicksSinceStart() {
  return clockPulseCount;
}

void TimeKeeper::internalTimerTick() {
  timerTicksSinceClockTick++;

  measure(); // do whatever we want


  // whenwever we get a pulse from outside, we recalculate the intervals of the internal clock
  if (clockHasTickedFlag) {
    clockPulseCount++;
    clockHasTickedFlag = false;
 //   if (clockPulseCount > 3) {

      unsigned long nClockTickEstimate = 1;
      // see if we might have missed a tick - otherwise don't waste time on this...
      if (TimeKeeper::timerTicksSinceClockTick > TimeKeeper::targetTimerTicksPerSecond ) {
        // round to closest integer
        nClockTickEstimate = (TimeKeeper::timerTicksSinceClockTick + ( TimeKeeper::targetTimerTicksPerSecond / 2ul)) / TimeKeeper::targetTimerTicksPerSecond;
      }
#ifdef TIME_PRESERVE_BALANCE
      // how many ticks is our internal time off sync in total?
      TimeKeeper::cumulatedTimerTickBalance += (long)TimeKeeper::timerTicksSinceClockTick - (long)(TimeKeeper::targetTimerTicksPerSecond * nClockTickEstimate);

      //how fast do we have to go to bring the balance to zero within two cycles?
      unsigned long targetTicksInNextClockInterval = constrain(((long)TimeKeeper::targetTimerTicksPerSecond) - ( TimeKeeper::cumulatedTimerTickBalance / nClockTickEstimate), targetTimerTicksPerSecond / 2, targetTimerTicksPerSecond * 2);
      unsigned long intervalForBalanceCorrection = TimeKeeper::curTimerInterval * TimeKeeper::timerTicksSinceClockTick /  targetTicksInNextClockInterval;

      TimeKeeper::curTimerInterval = constrain(intervalForBalanceCorrection, minTimerInterval, maxTimerInterval);
#else

      unsigned long intervalForCorrectRate = ( (TimeKeeper::curTimerInterval * TimeKeeper::timerTicksSinceClockTick) / nClockTickEstimate + targetTimerTicksPerSecond / 2) /  targetTimerTicksPerSecond;
      TimeKeeper::curTimerInterval = constrain(intervalForCorrectRate, minTimerInterval, maxTimerInterval);
#endif

      setTimerOnePeriod(TimeKeeper::curTimerInterval);
      reportTimerUpdate(nClockTickEstimate, timerTicksSinceClockTick);
   // }
    TimeKeeper::timerTicksSinceClockTick = 0;
  }
}

void TimeKeeper::externalClockTick() {
  if (!timerIsStartetFlag) {
    timerIsStartetFlag = true;
  } else {
    clockHasTickedFlag = true;
  }
}
void TimeKeeper::setup(unsigned long samplesPerSecond, int inputPin) {
  // calculate timer settings
  timerIsStartetFlag = false;
  TimeKeeper::targetTimerTicksPerSecond = samplesPerSecond;

  curTimerInterval = F_CPU  / samplesPerSecond / 2;
  maxTimerInterval = (curTimerInterval * 11) / 10; // we trust our internal oszillator to be within +-10% its actual frequency
  minTimerInterval = (curTimerInterval * 9) / 10;

  // prepare for receiving clock pulses
  attachInterrupt(digitalPinToInterrupt(inputPin), &clockPulseISR, RISING );
  initializeTimerOne(curTimerInterval); // start Timer
}

void TimeKeeper::reportTimerUpdate(unsigned long nClockIntervals, unsigned long nTimerTicks) {
  /*
    Serial.print(F("Updated timer after "));
    Serial.print(nTimerTicks);
    Serial.print(F(" ticks. balance is now"));
    Serial.print(cumulatedTimerTickBalance );
    Serial.print(F(". Next interval is"));
    Serial.print(TimeKeeper::curTimerInterval );
    Serial.print(F(" internal cycles.\n"));
  */
}


void timerTickISR() {

}

void clockPulseISR() {
  TimeKeeper::externalClockTick();
}


//////////////////////Timer stuff:
inline void setTimerOnePeriod(const unsigned long  cycles) {
  ICR1 = cycles;
}


inline void initializeTimerOne(const unsigned long  cycles) {
  unsigned char clockSelectBits = _BV(CS10);
  TCCR1B = _BV(WGM13) | clockSelectBits; // set mode as phase and frequency correct pwm, stop the timer + set prescale
  TCCR1A = 0;                 // clear control register A

  setTimerOnePeriod(cycles);

  TIMSK1 = _BV(TOIE1); // call ISR when ready...
}


//interropt of time one
ISR(TIMER1_OVF_vect)
{
  TimeKeeper::internalTimerTick();
}


