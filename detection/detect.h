#ifndef DETECT_H_
#define DETECT_H_
#include <stdio.h>


typedef struct {
	int count = -1;

	double start;
	double end;
}infoID;

int calculateFrequencies(ID id);

//Detect rate of incoming rate of detect frequencies 
void detectMsg();


#endif // DETECT_H_