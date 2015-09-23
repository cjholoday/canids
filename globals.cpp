#include "globals.h"



DigitalOut led1 (LED1);
DigitalOut led2 (LED2);
DigitalOut led3 (LED3);
DigitalOut led4 (LED4);

// We use can on mbed pins 29(CAN_TXD) and 30(CAN_RXD).
CAN can2(p30, p29);
CANMessage can_MsgRx;