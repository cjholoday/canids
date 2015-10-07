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
#include "SDFileSystem.h"

TextLCD lcd(p18, p19, p20, p17, p16, p15, p14); // rs, rw, e, d0, d1, d2, d3
SDFileSystem sd(p5, p6, p7, p13, "sd");

DigitalIn click(p21);   // Joystick inputs
DigitalIn right(p22);
DigitalIn down(p23);
DigitalIn left(p24);
DigitalIn up(p25);
Serial pc(USBTX, USBRX);


ecu_reader obdii(CANSPEED_500);     //Create object and set CAN speed
void sd_demo(void);

int main() {
    pc.baud(115200);
    char buffer[20];
    
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
    lcd.printf("www.google.com");
       
    pc.printf("\n\rCAN-bus demo...");
    
    wait(3);
    lcd.cls();
    lcd.printf("Use joystick");

    lcd.locate(0,1);
    lcd.printf("U-CAN:D-GPS:L-SD");
    
    pc.printf("\nU-CAN:D-GPS:L-SD");

    while(1)    // Wait until option is selected by the joystick
    {
   
        if(left == 0) sd_demo();
               
        if(up == 0) break;
        
    }
    lcd.cls();

    while(1) {  // Main CAN loop
        led2 = 1;
        wait(0.1);
        led2 = 0;
        wait(0.1);
        
        if(obdii.request(ENGINE_RPM,buffer) == 1)   // Get engine rpm and display on LCD
        {
            lcd.locate(0,0);
            lcd.printf(buffer);
              pc.printf(buffer);
        }   
         
        if(obdii.request(ENGINE_COOLANT_TEMP,buffer) == 1)
        {
            lcd.locate(9,0);
            lcd.printf(buffer);
        }
        
        if(obdii.request(VEHICLE_SPEED,buffer) == 1)
        {
            lcd.locate(0,1);
            lcd.printf(buffer);
        }
     
        if(obdii.request(THROTTLE,buffer) ==1 )
        {
            lcd.locate(9,1);
            lcd.printf(buffer);          
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
