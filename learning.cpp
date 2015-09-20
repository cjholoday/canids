#include "learning.h"
#include "mbed.h"
#include "ecu_reader.h"
#include "globals.h"
#include "TextLCD.h"
#include "GPS.h"
#include "SDFileSystem.h"
#include <errno.h> //system error numbers

#define CANDUMP_PATH "/sd/messagestore.txt"
#define ARRAY_SIZE 0x7FF // 2048 bits for all common hexcode
#define COLLECTION_TIME_MS 5000

//for unordered_map
using namespace std;

TextLCD lcd(p18, p19, p20, p17, p16, p15, p14); // rs, rw, e, d0, d1, d2, d3
SDFileSystem sd(p5, p6, p7, p13, "sd");

Serial pc(USBTX, USBRX);

ecu_reader obdii(CANSPEED_500);     //Create object and set CAN speed

int main() {

    pc.baud(115200);

    message_reader();
}

void message_reader(){
    int counterID[ARRAY_SIZE]; // preallocates memory  for the hexcode ID
    for(int i = 0; i < ARRAY_SIZE; i++){
        counterID[i] = 0; // creates the hexcode ID
    }
    timer.start(); //Start the timer
    CANMessage myMessage;
    while (timer.read_ms() < COLLECTION_TIME_MS){
        if (can2.read(myMessage)) {
            if (myMessage.id != 0){
        counterID[myMessage.id]++; // counts number IDs that have passed
        }
        led1 = !led1; // led flash while in use
        }
    }
    
    timer.stop();

    /*
        TO DO:
            Write store_messages()
            Call that instead of storing in text file below
    */
    FILE *fp = fopen(CANDUMP_PATH, "w"); // create a writable file "messagestore"
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
        fprintf(fp,"%x %f\n", id, counterID[id]/totTime); // prints ID and ID frequancy
    }

    fclose(fp);
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("file completed");
    
    //template file written, complete more tasks
}