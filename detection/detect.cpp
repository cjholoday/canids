#include "detect.h"
#include "globals.h"
#include <errno.h> //system error numbers
#include <memory>
#include <ctime>
#include <stdlib.h>

void detectMsg(){
    printf("Begin detection");

    infoID all_IDs[ARRAY_SIZE]
    memset(all_IDs, 0, ARRAY_SIZE);
    std::clock_t start;

    start = std::clock();

    while (1){

        
        printf("In loop");
        if (can.messageAvailable()) {
            can.getMessage(&can.messageRx);
            if (can.messageRx.id != 0){
		        //If we haven't seen this CAN message yet, add it to the table of seen messages
                if(infoID[can.messageRx.id] == -1){
                    ID new_ID;
                    new_ID.count = 1;
                    new_ID.id = can.messageRx.id;
                    new_ID.start = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;	//current time we have seen the message

                    all_IDs[current.id] = new_ID;
                    continue;
                }

		        //If we have seen this message at least 10 times, then calculate its frequency
                if(++(all_IDs[current.id].count) >= 10){
                    ID * cur = &(all_IDs[current.id]);
                    cur->end = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

		            /*
		            * message frequency = (time first seen - time last seen) / (times message seen)
 	                */
                    double freq = (cur->start - cur->end) / cur->count;
                    double lower_bound = frequencies[current.id] * 0.9;
                    double upper_bound = frequencies[current.id] * 1.1;

                    if (lower_bound <= freq && freq <= upper_bound){
                        continue;
                    }
                    else{
                        mitigate();
                    }
                }
            }
        }
    }
    
}

void mitigate(){
    for (int i = 0; i < 10; i++){
        printf("Mitigating attack");
        sleep(2000);
    }
}

