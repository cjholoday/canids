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
#include "learning.h"
#include "detect.h"

ecu_reader obdii(CANSPEED_500);     //Create object and set CAN speed

int main() {
    pc.baud(115200);
    char buffer[20];
    
    //Enable Pullup 
    click.mode(PullUp);
    right.mode(PullUp);
    down.mode(PullUp);
    left.mode(PullUp);
    up.mode(PullUp);
    
    //printf("Automotive IDS \n"); 
    lcd.locate(0,0);                // Set LCD cursor position
    lcd.printf("Automotive IDS");
    
    lcd.locate(0,1);
    lcd.printf("UMTRI");
       
    pc.printf("\n\rAutomotive IDS initializing...");
    
    wait(3);
    lcd.cls();
    lcd.printf("Use joystick");
    messageReader();

    while(1)    // Wait until option is selected by the joystick
    {        

        if (right == 0){
            detectMsg();
        }
        
    }
    lcd.cls();

    while(1) {  // Main CAN loop
        led2 = !led2;
        wait(0.1);
        led2 = !led2;
        wait(0.1);
        
        if(obdii.request(ENGINE_RPM,buffer,NULL,NULL,NULL) == 1)   // Get engine rpm and display on LCD
        {
            lcd.locate(0,0);
            lcd.printf(buffer);
            pc.printf(buffer);
        }   
         
        if(obdii.request(ENGINE_COOLANT_TEMP,buffer,NULL,NULL,NULL) == 1)
        {
            lcd.locate(9,0);
            lcd.printf(buffer);
        }
        
        if(obdii.request(VEHICLE_SPEED,buffer,NULL,NULL,NULL) == 1)
        {
            lcd.locate(0,1);
            lcd.printf(buffer);
        }
     
        if(obdii.request(THROTTLE,buffer,NULL,NULL,NULL) ==1 )
        {
            lcd.locate(9,1);
            lcd.printf(buffer);          
        }   
       
    }
}
