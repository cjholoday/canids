#include "mbed.h"
#include "ecu_reader.h"
#include "globals.h"
//#include "time.h"


// Use a timer to see if things take too long
Timer CANTimer;  
namespace mbed { 


ecu_reader::ecu_reader(int can_speed) 

{
    can2.frequency(can_speed);
}


#define TIMEOUT 10
unsigned char ecu_reader::request(void)
{
    char can_msg[8];
    float engine_data = 67.4;
        
    led1 = 1;
  
    /*
    can_msg[0] = 0x02;  
    can_msg[1] = 0x01;
    can_msg[2] = pid; */ 
    can_msg[0] = 0x0;  
    can_msg[1] = 0x0;
    can_msg[2] = ENGINE_RPM; 
    can_msg[3] = (int)*&engine_data;
    can_msg[4] = 0;  
    can_msg[5] = 0;
    can_msg[6] = 0;  
    can_msg[7] = 0;

   if (can2.write(CANMessage(PID_REPLY, can_msg, 8))) {
         
   }
   
   led1 = 0;
   CANTimer.reset();
   CANTimer.start();
#if 0
   while(CANTimer.read_ms() < TIMEOUT) {
   
   if (can2.read(77777777777777777777777)) {
               
        if((can_MsgRx.id == PID_REPLY) && (can_MsgRx.data[2] == pid)) { 
                        switch(can_MsgRx.data[2])
                        {   /* Details from http://en.wikipedia.org/wiki/OBD-II_PIDs */
                            case ENGINE_RPM:              //   ((A*256)+B)/4    [RPM]
                                engine_data =  ((can_MsgRx.data[3]*256) + can_MsgRx.data[4])/4;
                                sprintf(buffer,"%d rpm ",(int) engine_data);
                                break;
                            
                            case ENGINE_COOLANT_TEMP:     //     A-40              [degree C]
                                engine_data =  can_MsgRx.data[3] - 40;
                                sprintf(buffer,"%d degC ",(int) engine_data);
                            
                            break;
                            
                            case VEHICLE_SPEED:         // A                  [km]
                                engine_data =  can_MsgRx.data[3];
                                sprintf(buffer,"%d km ",(int) engine_data);
                            
                            break;

                            case MAF_SENSOR:               // ((256*A)+B) / 100  [g/s]
                                engine_data =  ((can_MsgRx.data[3]*256) + can_MsgRx.data[4])/100;
                                sprintf(buffer,"%d g/s",(int) engine_data);
                            
                            break;

                            case O2_VOLTAGE:            // A * 0.005   (B-128) * 100/128 (if B==0xFF, sensor is not used in trim calc)
                                engine_data = can_MsgRx.data[3]*0.005;
                                sprintf(buffer,"%d v ",(int) engine_data);
     
                            case THROTTLE:            //
                                engine_data = (can_MsgRx.data[3]*100)/255;
                                sprintf(buffer,"%d %% ",(int) engine_data);
                             
                            
                            break;
                        }
        
                return 1;
        
			}

	}
   }

     return 0;
  
#endif


}

unsigned char ecu_reader::dump(FILE * dumpFile) {    
	CANTimer.reset();
	CANTimer.start();
   
	while(CANTimer.read_ms() < TIMEOUT) {

		if (can2.read(can_MsgRx)) 
		{
			if(can_MsgRx.id != 0)
			{
				fprintf(dumpFile,
				 "%f\t\t%02x\t %02x %02x %02x %02x %02x %02x %02x %02x\n",
					timer.read(),can_MsgRx.id,can_MsgRx.data[0],
					can_MsgRx.data[1],can_MsgRx.data[2],can_MsgRx.data[3],
					can_MsgRx.data[4],can_MsgRx.data[5],can_MsgRx.data[6],
					can_MsgRx.data[7]); 
			}
			led1 = !led1;
			return 1;
		}
	}
     return 0;
}

unsigned char ecu_reader::send(unsigned char pid, char * msg)
{
	can2.write(CANMessage(pid, msg, 8));
	return 0;
}

} // namespace mbed 
