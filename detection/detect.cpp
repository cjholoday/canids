#include "detect.h"
#include "globals.h"
#include <errno.h> //system error numbers
#include <memory>
#include <ctime>
#include <stdlib.h>
#include "../can-utils/lib.h"

int detectMsg(){
    printf("Begin detection");

    std::map<canid_t, infoID> all_IDs;

    std::clock_t start;

    start = std::clock();

    struct sockaddr_can addr;
    struct ifreq ifr;
    socklen_t len = sizeof(addr);
    struct can_frame frame;


    while (1){

        can_bytes = recvfrom(s, &frame, sizeof(struct can_frame),
                  0, (struct sockaddr*)&addr, &len);

        /* get interface name of the received CAN frame */
        ifr.ifr_ifindex = addr.can_ifindex;
        ioctl(s, SIOCGIFNAME, &ifr);

        if(can_bytes < 0) {
            perror("can raw socket read");
            return 1;
        }

        if (can_bytes < sizeof(struct can_frame)) {
            fprintf(stderr, "read: incomplete CAN frame\n");
            return 1;
        }
        
        canid_t current_id = frame.can_id;
        std::map<caN_id, infoID>::iterator it = all_IDs.find(current_id);

        //If we haven't seen this CAN message yet, add it to the table of seen messages
        if(it == all_IDs.end()){
            ID new_ID;
            new_ID.count = 1;
            new_ID.start = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;	//current time we have seen the message

            all_IDs[current_id] = new_ID;
            continue;
        }

        //If we have seen this message at least 10 times, then calculate its frequency
        if(++(all_IDs[current_id].count) >= 10){
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
                printf("Attack detected");
            }
        }
    }
}


