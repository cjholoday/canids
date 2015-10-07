#include "learning.h"
#include "detect.h"
#include "mbed.h"
#include "ecu_reader.h"
#include "globals.h"
#include "TextLCD.h"
#include "GPS.h"
#include "SDFileSystem.h"
#include "globals.h"
#include <errno.h> //system error numbers

void calculateFrequencies(){}

void detectMsg(){
    ID all_IDs[ARRAY_SIZE]; 
    ID blank;
    blank.count = -1;
    memset(all_IDs, 0, ARRAY_SIZE);
    CANMessage current;

    while (1){
        if (can2.read(current)) {
            if (current.id != 0){
            	if(all_IDs[current.id].count == -1){
            		ID new_ID;
            		new_ID.count = 1;
            		new_ID.msg = current;

            		all_IDs[current.id] = new_ID;
            		continue;
            	}
            	if(++(all_IDs[current.id].count) >= 10){
            		int freq = calculateFrequencies(all_IDs[current.id]);
            		if (freq){
            			continue;
            		}
            		/*
            		Compare freq with look up table, if bad then call mitigate.
            		*/
            	}
        	}
        }
    }
    
}

void mitigate(){

}

