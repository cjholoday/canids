#ifndef DETECT_H_
#define DETECT_H_
#include "mbed.h"

struct ID{
	CANMessage msg;
	int count;

	int start;
	int end;
};

int calculateFrequencies(ID id);

//Detect rate of incoming rate of detect frequencies 
void detectMsg();

//Do some kind of prevention idk we haven't thought this far
void mitigate();


#endif // DETECT_H_