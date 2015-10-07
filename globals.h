#ifndef GLOBALS_H
#define GLOBALS_H

#include "mbed.h"

#define ARRAY_SIZE 0x7FF 


extern DigitalOut led1;
extern DigitalOut led2;
extern DigitalOut led3;
extern DigitalOut led4;

// We use can on mbed pins 29(CAN_TXD) and 30(CAN_RXD).
extern CAN can2;
extern CANMessage can_MsgRx;
#endif