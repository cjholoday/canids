/*
 * File will need to be edited to work with the new Raspberry Pis.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include "mbed.h"
#include "TextLCD.h"
#include "SDFileSystem.h"
#include <unordered_map>

#define ARRAY_SIZE 0x7FF 

<<<<<<< HEAD
#define CANSPEED_125      125000        // CAN speed at 125 kbps
#define CANSPEED_250      250000        // CAN speed at 250 kbps
#define CANSPEED_500      500000        // CAN speed at 500 kbps

extern Timer timer;
=======
unordered_map<int, double> frequencies;
>>>>>>> 82066e09817ed3057ccfc59603c37002b7cb4aa2

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
