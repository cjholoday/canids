#include "learning.h"
#include "detect.h"
#include "mbed.h"
#include "ecu_reader.h"
#include "globals.h"
#include "TextLCD.h"
#include "globals.h"
#include <errno.h> //system error numbers

void detectMsg(){
    ID all_IDs[ARRAY_SIZE]; 
    ID blank;
    blank.count = -1;
    memset(all_IDs, 0, ARRAY_SIZE);
    CANMessage current;

    timer.start();
    while (1){
        if (can2.read(current)) {
            if (current.id != 0){
            	if(all_IDs[current.id].count == -1){
            		ID new_ID;
            		new_ID.count = 1;
            		new_ID.msg = current;
                    new_ID.start = timer.read_ms();

            		all_IDs[current.id] = new_ID;
            		continue;
            	}

            	if(++(all_IDs[current.id].count) >= 10){
                    ID * cur = &(all_IDs[current.id]);
                    cur->end = timer.read_ms();
            		double freq = (cur->start - cur->end) / cur->count;
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

