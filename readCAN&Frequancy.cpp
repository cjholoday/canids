/*

mbed Can-Bus demo

This program is to demonstrate the CAN-bus capability of the mbed module.

http://www.skpang.co.uk/catalog/product_info.php?products_id=741

v1.0 July 2010

********************************************************************************

WARNING: Use at your own risk, sadly this software comes with no guarantees.
This software is provided 'free' and in good faith, but the author does not
accept liability for any damage arising from its use.

********************************************************************************


*/

#include "mbed.h"
#include "ecu_reader.h"
#include "globals.h"
#include "TextLCD.h"
#include "GPS.h"
#include "SDFileSystem.h"
#include <errno.h>
#define CANDUMP_PATH "/sd/CANdump.txt"
#define ARRAY_SIZE 0x7FF // 2048 bits for all common hexcode
#define COLLECTION_TIME_MS 5000

GPS gps(p28, p27);
TextLCD lcd(p18, p19, p20, p17, p16, p15, p14); // rs, rw, e, d0, d1, d2, d3
SDFileSystem sd(p5, p6, p7, p13, "sd");

DigitalIn click(p21);   // Joystick inputs
DigitalIn right(p22);
DigitalIn down(p23);
DigitalIn left(p24);
DigitalIn up(p25);
Serial pc(USBTX, USBRX);


ecu_reader obdii(CANSPEED_500);     //Create object and set CAN speed
void gps_demo(void);
void sd_demo(void);
//void send_demo(void);
void attempt_engine(void);
Timer timer;
void message_reader(void);

int main() {
    pc.baud(115200);
    //char buffer[20];
    
    //Enable Pullup 
    click.mode(PullUp);
    right.mode(PullUp);
    down.mode(PullUp);
    left.mode(PullUp);
    up.mode(PullUp);
    
    printf("ECU Reader \n"); 
    lcd.locate(0,0);                // Set LCD cursor position
    lcd.printf("CAN-Bus demo");
    
    lcd.locate(0,1);
    lcd.printf("Updated");
       
    pc.printf("\n\rCAN-bus demo...");
    
    wait(3);
    lcd.cls();
    lcd.printf("Use joystick");

    lcd.locate(0,1);
    lcd.printf("U-CAN:D-REQ:L-SD");
    
    pc.printf("\nU-CAN:D-REQ:L-SD");

    while(1){    // Wait until option is selected by the joystick
        if(down == 0) attempt_engine();
        if(left == 0) sd_demo();
        if(right == 0) message_reader();
        if(up == 0) break;
        
    }
	FILE *fp = fopen(CANDUMP_PATH, "a");
    lcd.cls();
	lcd.locate(0,0);
    lcd.printf("Dumping CAN...");
    lcd.locate(0,1);
    lcd.printf("press joystick");
    timer.start();
    while(fp != NULL) {  // Main CAN loop
        led2 = 1;
        wait(0.1);
        led2 = 0;
        wait(0.1);
        obdii.dump(fp);
        if(click == 0) break;
        lcd.locate(0,0);
        lcd.printf("timer: %f", timer.read());
    }
    if(fp  == NULL){
		lcd.cls();
		lcd.locate(0,0);
		lcd.printf("File pointer issue");
	}
	fclose(fp);
	lcd.cls();
	lcd.locate(0,0);
	lcd.printf("safe to remove");
	lcd.locate(0,1);
	lcd.printf("SD card. ");
}

void gps_demo(void){
    lcd.cls();
    lcd.printf("GPS demo");
    lcd.locate(0,1);
    lcd.printf("Waiting for lock");
 
    wait(3);    
    lcd.cls();
   
    while(1){
      if(gps.sample()) {
        lcd.cls();
        lcd.printf("Long:%f", gps.longitude);
           lcd.locate(0,1);
        lcd.printf("Lat:%f", gps.latitude);
            pc.printf("I'm at %f, %f\n", gps.longitude, gps.latitude);
        } else {
            pc.printf("Oh Dear! No lock :(\n");
            lcd.cls();
            lcd.printf("Waiting for lock");
   
        }
    }
 
}

void sd_demo(void){
    lcd.cls();
     printf("\nSD demo");
    lcd.printf("SD demo");
    wait(2);      
    lcd.cls();
    
    FILE *fp = fopen("/sd/sdtest2.txt", "w");
    if(fp == NULL) {
        lcd.cls();
        lcd.printf("Could not open file for write\n");
         pc.printf("\nCould not open file for write");
    }
    fprintf(fp, "Hello fun SD Card World! testing 1234");
    fclose(fp); 
    lcd.locate(0,1);
    lcd.printf("Writtern to SD card");
    pc.printf("\nWrittern to SD card");
        
    while(1){
        led2 = 1;
        wait(0.1);
        led2 = 0;
        wait(0.1);
   
    }
 
}

void attempt_engine(){
	int counterID[ARRAY_SIZE]; //
	memset(counterID,ARRAY_SIZE*sizeof(int),0);
	while(1){
		obdii.request();
		wait(0.1);
	}
}

void message_reader(){
	
	int counterID[ARRAY_SIZE]; // preallocates memory  for the hexcode ID
	for(int i = 0; i < ARRAY_SIZE; i++){
		counterID[i] = 0; // creates the hexcode ID
	}
	timer.start();//Start the timer.
	CANMessage myMessage;
	while (timer.read_ms() < COLLECTION_TIME_MS){
		if (can2.read(myMessage)) {
			if(myMessage.id != 0){
				counterID[myMessage.id]++; // counts number IDs that have passed
			}
			led1 = !led1; // led flash while in use
		}
	}
	
	timer.stop();
	FILE *fp = fopen("/sd/messagestore.txt", "w"); // create a writable file "messagestore"
	lcd.locate(0,1);
	if (fp == NULL){
		lcd.printf("file open failed %d", errno);
		return;
	}
	
	double totTime;
	totTime = timer.read_ms(); // read time lapse in milliseconds
	lcd.printf("%f\t\n", totTime);
	for (unsigned int id = 0; id < ARRAY_SIZE; id++){
		//fprintf(fp,"0x%x \t\t 0x%x \t\t %f\n", id,counterID[id],counterID[id]/totTime*1000);
		fprintf(fp,"%x\t\t %f", id, counterID[id]/totTime); // prints ID and ID frequancy
	}
	fclose(fp);
	lcd.cls();
	lcd.locate(0,0);
	lcd.printf("file completed");
	//template file written, complete more tasks
}
