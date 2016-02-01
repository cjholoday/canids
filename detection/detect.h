#ifndef DETECT_H_
#define DETECT_H_
#include "CAN.h"
#include <stdio.h>


typedef struct {
	unsigned int id;
	int count = -1;

	double start;
	double end;
}infoID;

int calculateFrequencies(ID id);

//Detect rate of incoming rate of detect frequencies 
void detectMsg();

//Do some kind of prevention idk we haven't thought this far
void mitigate();


#endif // DETECT_H_