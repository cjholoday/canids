#ifndef ECU_READER_H
#define ECU_READER_H

#define CANSPEED_125      125000        // CAN speed at 125 kbps
#define CANSPEED_250      250000        // CAN speed at 250 kbps
#define CANSPEED_500      500000        // CAN speed at 500 kbps

 /* Details from http://en.wikipedia.org/wiki/OBD-II_PIDs */
#define ENGINE_COOLANT_TEMP 0x05
#define ENGINE_RPM          0x0C
#define VEHICLE_SPEED       0x0D
#define MAF_SENSOR          0x10
#define THROTTLE            0x11
#define O2_VOLTAGE          0x14

#define PID_REQUEST         0x7DF
#define PID_REPLY           0x7E8

namespace mbed { 

class ecu_reader{

public:

    ecu_reader(int can_speed);

    unsigned char request(unsigned char pid,  char *buffer);

private: 

    int i;
 
};





    } 



#endif