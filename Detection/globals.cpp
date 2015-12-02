#include "globals.h"

Serial pc(USBTX,USBRX);
TextLCD lcd(p18, p19, p20, p17, p16, p15, p14); // rs, rw, e, d0, d1, d2, d3
SDFileSystem sd(p5, p6, p7, p13, "sd");

DigitalOut led1 (LED1);
DigitalOut led2 (LED2);
DigitalOut led3 (LED3);
DigitalOut led4 (LED4);

DigitalIn click(p21);   // Joystick inputs
DigitalIn right(p22);
DigitalIn down(p23);
DigitalIn left(p24);
DigitalIn up(p25);

// We use can on mbed pins 29(CAN_TXD) and 30(CAN_RXD).
CAN can2(p30, p29);
CANMessage can_MsgRx;
int PID020, PID2140, PID4160;

Timer timer;

unordered_map<int, double> frequencies;