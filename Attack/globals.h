#ifndef GLOBALS_H
#define GLOBALS_H

#include "mbed.h"
#include "TextLCD.h"
#include "SDFileSystem.h"
#include <unordered_map>

#define ARRAY_SIZE 0x7FF 

unordered_map<int, double> frequencies;

extern Serial pc;
extern TextLCD lcd;
extern SDFileSystem sd;

extern DigitalOut led1;
extern DigitalOut led2;
extern DigitalOut led3;
extern DigitalOut led4;
extern DigitalIn click;   // Joystick inputs
extern DigitalIn right;
extern DigitalIn down;
extern DigitalIn left;
extern DigitalIn up;

// We use can on mbed pins 29(CAN_TXD) and 30(CAN_RXD).
extern CAN can2;
extern CANMessage can_MsgRx;
extern int PID020;
extern int PID2140;
extern int PID4160; //PID Support Masks
#endif // GLOBALS_H_