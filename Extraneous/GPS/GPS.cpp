/* GPS class for mbed Microcontroller
 * Copyright (c) 2008, sford
 */

#include "GPS.h"

GPS::GPS(PinName tx, PinName rx) : _gps(tx, rx) {
	_gps.baud(4800);	
	longitude = 0.0;
	latitude = 0.0;		
}


int GPS::sample() {
	float time;
	char ns, ew;
	int lock;

	while(1) {		
		getline();

		// Check if it is a GPGGA msg (matches both locked and non-locked msg)
		if(sscanf(msg, "GPGGA,%f,%f,%c,%f,%c,%d", &time, &latitude, &ns, &longitude, &ew, &lock) >= 1) { 
			if(!lock) {
				longitude = 0.0;
				latitude = 0.0;		
				return 0;
			} else {
				if(ns == 'S') {	latitude  *= -1.0; }
				if(ew == 'W') {	longitude *= -1.0; }
				float degrees = trunc(latitude / 100.0f);
				float minutes = latitude - (degrees * 100.0f);
				latitude = degrees + minutes / 60.0f;	
				degrees = trunc(longitude / 100.0f * 0.01f);
				minutes = longitude - (degrees * 100.0f);
				longitude = degrees + minutes / 60.0f;
				return 1;
			}
		}
	}
}

float GPS::trunc(float v) {
	if(v < 0.0) {
		v*= -1.0;
		v = floor(v);
		v*=-1.0;
	} else {
		v = floor(v);
	}
	return v;
}

void GPS::getline() {
	while(_gps.getc() != '$');	// wait for the start of a line
	for(int i=0; i<256; i++) {
		msg[i] = _gps.getc();
		if(msg[i] == '\r') {
			msg[i] = 0;
			return;
		}
	}
	error("Overflowed message limit");
}

