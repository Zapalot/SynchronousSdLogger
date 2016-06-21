# GPS-Synchronized Datalogging

This is an attempt to create an SD-datalogger with <=1ms global timing accuracy.

## Key features:
	* Synchronization is provided by the 1PPS signal of a GPS-Module.
	* Sampling is done in a timer-interrupt. Timer intervals are adjusted to keep sampling at a given rate even if the internal clock drifts.
	* Data including time-stamps are written to an SD-card in a human-friendly format

## Requirements
	* Tested on an Arduino Leonardo using the Adafruit GPS-Logger-Shield.
	* The additional Memory of a MEGA would make things simpler.
	* Other SD-Adaptors and GPS-Modules should work just as well.
	* Both the SD-Library [https://github.com/adafruit/SD] and the Adafruit-GPS [https://github.com/adafruit/Adafruit_GPS] Library are needed for compilation.

## Connections
Connections on the shield must be modified to use..
	* Hardware UART for communication with the GPS-Module
	* Hardware SPI for the SD-Card
	* 1PPS Signal to Digital Pin 2 or any other Input-Interrupt capable pin of your µC.

## Known issues:
	* Samples get lost during SD-write operations because of insufficient memory for buffering. (on Leonardo/Uno)
	* The GPS-Library is a bit of a fatso and steals a lot of memory - it should be replaced by something more barebone ASAP. 
	* The SD-Library will often break the filesystem on the SD-card if the file is not closed before rebooting/shutting down the Arduino
	* Due to it beeing written in a hurry, in-code documentation has room for improvements
	