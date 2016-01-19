/* GPS class for mbed Microcontroller
 * Copyright (c) 2008, sford
 */

#include "mbed.h"

#ifndef GPS_H
#define GPS_H

/* Class: GPS
 *  A GPS interface for reading from a Globalsat EM-406 GPS Module
 */
class GPS {

public:

	/* Constructor: GPS
	 *  Create the GPS, connected to the specified serial port
	 */	
	GPS(PinName tx, PinName rx);
	
	/* Function: sample
	 *  Sample the incoming GPS data, returning whether there is a lock
	 * 
	 * Variables:
	 *  returns - 1 if there was a lock when the sample was taken (and therefore .longitude and .latitude are valid), else 0
	 */
	int sample();
	
	/* Variable: longitude
	 *  The longitude (call sample() to set)
	 */
	float longitude;

	/* Variable: latitude
	 *  The latitude (call sample() to set)
	 */
	float latitude;
	
private:

	float trunc(float v);
	void getline();
	
	Serial _gps;
	char msg[256];

};

#endif
