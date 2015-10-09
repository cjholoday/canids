#ifndef GLOBALS_H
#define GLOBALS_H

#include "mbed.h"
#include "TextLCD.h"

#define ARRAY_SIZE 0x7FF 

extern Serial pc;
extern TextLCD lcd;
extern DigitalOut led1;
extern DigitalOut led2;
extern DigitalOut led3;
extern DigitalOut led4;

// We use can on mbed pins 29(CAN_TXD) and 30(CAN_RXD).
extern CAN can2;
extern CANMessage can_MsgRx;
extern int PID020;
extern int PID2140;
extern int PID4160; //PID Support Masks
#endif