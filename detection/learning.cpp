#include "learning.h"
//#include "mbed.h"
//#include "ecu_reader.h"
#include "globals.h"
#include <errno.h> //system error numbers
#include "linux/can.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "stdio.h"
#include "string.h"

//change the path?
#define CANDUMP_PATH "/sd/messagestore.txt"
#define ARRAY_SIZE 0x7FF // 2048 bits for all common hexcode
#define COLLECTION_TIME_SEC 5

//for unordered_map
using namespace std;

void storeMessages(FILE *fp){
    //lcd.locate(0,1);
    printf("Storage starting");
    double frequency;
    int CAN_ID;
    int num = sizeof(double) + sizeof(unsigned int);
    char * buf = new char[num];
    memset(buf, 0, num);
    while (! feof (fp)){
        if (fgets(buf, sizeof(unsigned int), fp) == NULL)
            break;
        CAN_ID = int(buf);

        if (fgets(buf, sizeof(double), fp) == NULL)
            break;
        frequency = atof(buf);

        frequencies[CAN_ID] = frequency;
    }
    //lcd.locate(0,0);
    printf("Storage completed");
}

int messageReader(){
    //lcd.locate(0,0);
    printf("Reader beginning");
    canid_t counterID[ARRAY_SIZE]; // preallocates memory  for the hexcode ID
    for(unsigned int i = 0; i < ARRAY_SIZE; i++){
        counterID[i] = 0; // creates the hexcode ID
    }
    
    //copied from can.txt, creating and binding a socket s
    int s;
    struct sockaddr_can addr; 
    struct ifreq ifr;
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "can0" ); //where is can0? or should it be can2?
    ioctl(s, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN; 
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    
    /* change below
    CANMessage myMessage;
    while (timer.read_ms() < COLLECTION_TIME_MS){
        if (can2.read(myMessage)) {
            if (myMessage.id != 0){
        counterID[myMessage.id]++; // counts number IDs that have passed
        }
        led1 = !led1; // led flash while in use
        }
    } */

    //timer.start(); //Start the timer
    clock_t start = clock();
    clock_t end = clock();

    struct can_frame frame;
    while (1){
	    nbytes = read(s, &frame, sizeof(struct can_frame));
	    if (nbytes < 0) {
		    perror("can raw socket read");
		    return 1;
	    }
	    /* paranoid check ... */
	    if (nbytes < sizeof(struct can_frame)) {
		    fprintf(stderr, "read: incomplete CAN frame\n"); 
		    return 1;
	    }
	    /* do something with the received CAN frame */
	    if (frame.can_id != 0){
		    counterID[frame.can_id]++;
	    }
	    end = clock();
	    if ((end - start) / CLOCKS_PER_SEC >= COLLECTION_TIME_SEC) break;
	    start = end;
	    //led flash? (can use gpio pins if have to)
    }

    //timer.stop();

    FILE *fp = fopen(CANDUMP_PATH, "w"); // create a writable file "messagestore"
    //lcd.locate(0,1);
    if (fp == NULL){
        printf("file open failed %d", errno);
    return 2;
    }
    
    double totTime = (double) (end - start) / CLOCKS_PER_SEC * 1000;
    //totTime = timer.read_ms(); // read time lapse in milliseconds
    printf("%f\t\n", totTime);
    for (unsigned int id = 0; id < ARRAY_SIZE; id++){
        //fprintf(fp,"0x%x \t\t 0x%x \t\t %f\n", id,counterID[id],counterID[id]/totTime*1000);
        fprintf(fp,"%x %f\n", id, double(counterID[id]/totTime)); // prints ID and ID frequancy
    }

    //locate(0,1);
    printf("calling storage");
    storeMessages(fp);

    fclose(fp);
    //lcd.cls();
    //lcd.locate(0,0);
    printf("file completed");
    
    //template file written, complete more tasks
    return 0;
}
