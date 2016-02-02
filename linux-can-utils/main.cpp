#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "lib.h"

void attacks(void);

int main() {

    attacks();
    return 0;
}

int send(char buff[], int &s){
	struct can_frame frame;
	int nbytes;

	/* parse buffer into can frame */
	if (parse_canframe(buff, &frame)){
		return 0;
	}

	/* send frame */
	if ((nbytes = write(s, &frame, sizeof(frame))) != sizeof(frame)) {
		perror("write");
		return 0;
	}
	return 1;
}

void attacks(void){
	printf("\n up/down for headlights, left/right for fan");
	
	int s; /* can raw socket */ 
	struct sockaddr_can addr;
	struct can_frame frame;
	struct ifreq ifr;

	/* open socket */
	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("socket");
		close(s);
		exit(1);
	}

	addr.can_family = AF_CAN;

	//strncpy(ifr.ifr_name, argv[1]);
	if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
		perror("SIOCGIFINDEX");
		close(s);
		exit(1);
	}
	addr.can_ifindex = ifr.ifr_ifindex;

	/* disable default receive filter on this RAW socket */
	/* This is obsolete as we do not read from the socket at all, but for */
	/* this reason we can remove the receive list in the Kernel to save a */
	/* little (really a very little!) CPU usage.                          */
	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		close(s);
		exit(1);
	}

	while(1)
		{

			}

	close(s);
}


			/*
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