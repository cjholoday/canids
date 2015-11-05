#include "mbed.h"
#include "ecu_reader.h"
#include "globals.h"
#include "TextLCD.h"

// Use a timer to see if things take too long
Timer CANTimer;  
namespace mbed { 


ecu_reader::ecu_reader(int can_speed)
{
    can2.frequency(can_speed);
}


#define TIMEOUT 200
unsigned char ecu_reader::request(unsigned char pid,  char *buffer, char *buffer2, char *buffer3, char *buffer4)
{
    char can_msg[8];
    float engine_data;
        
    led1 = 1;
  
    can_msg[0] = 0x02;  
    can_msg[1] = 0x01;
    can_msg[2] = pid;  
    can_msg[3] = 0;
    can_msg[4] = 0;  
    can_msg[5] = 0;
    can_msg[6] = 0;  
    can_msg[7] = 0;

    lcd.cls();
    lcd.locate(0,0); 
    if (can2.write(CANMessage(PID_REQUEST, can_msg, 8))) {
        lcd.printf("*********Request write passed*********\n\r");
        pc.printf("*********Request write passed*********\n\r");
    } else {
        lcd.printf("*********Request write failed*********\n\r");
        pc.printf("*********Request write failed*********\n\r");
    }
   
   led1 = 0;
   CANTimer.reset();
   CANTimer.start();
   
   while (CANTimer.read_ms() < TIMEOUT) {
        pc.printf("CANTimer.read_ms(): %dms ", CANTimer.read_ms());
 
        if (can2.read(can_MsgRx)) {
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("Message read!\n");
            pc.printf("Message read\n\r");
            
            //print message id
            pc.printf("can_MsgRx.id: %x\n\r\n\r", can_MsgRx.id);
 
            //print length of message
            pc.printf("Hex: can_MsgRx.len: %x\n\r", can_MsgRx.len);
 
            //print data[2] and PID
            pc.printf("can_MsgRx.data[2]: %x, pid: %x\n\r", can_MsgRx.data[2], pid);
 
            for (int i = 0; i < (int)can_MsgRx.len; i++) {
                pc.printf("can_MsgRx.data[%d]: %x\n\r", i, can_MsgRx.data[i]);
            }
 
            if ((can_MsgRx.id == PID_REPLY) && (can_MsgRx.data[2] == pid)) {
                pc.printf("Valid OBD-II PID reply\n\r");
                /* Details from http://en.wikipedia.org/wiki/OBD-II_PIDs */
                switch (can_MsgRx.data[2]) {                /* Details from http://en.wikipedia.org/wiki/OBD-II_PIDs */
                    case PID_0_20:                          // PID 0-20 Supported
                        PID020 = ((can_MsgRx.data[3] << 24) | (can_MsgRx.data[4] << 16) | (can_MsgRx.data[5] << 8) | (can_MsgRx.data[6]));
                        break;
                    case STATUS_DTC: {                      // bit encoded
                        if (can_MsgRx.data[4] & 0x04) {     //Compression Ignition (Diesel)
                            if (can_MsgRx.data[3] & 0x80) { //MIL Light on
                                engine_data = (can_MsgRx.data[3] - 128);
                                sprintf(buffer,"MIL ON, %d DTCs", (int) engine_data);
                            } else {                        //MIL Light off
                                engine_data = (can_MsgRx.data[3]);
                                sprintf(buffer,"MIL OFF, %d DTCs", (int) engine_data);
                            }
                            // Diesel C and D bytes (can_MsgRx.data[5] and can_MsgRx.data[6])
                            //                      Test available  Test incomplete
                            // Catalyst             C0              D0
                            // Heated Catalyst      C1              D1
                            // Evap System          C2              D2
                            // Secondary Air        C3              D3
                            // A/C Refrigerant      C4              D4
                            // O2 Sensor            C5              D5
                            // O2 Sensor Heater     C6              D6
                            // EGR System           C7              D7
                        } else {                            //Spark Ignition (Gasoline)
                            if (can_MsgRx.data[3] & 0x80) { //MIL Light on
                                engine_data = (can_MsgRx.data[3] - 128);
                                sprintf(buffer,"MIL ON, %d DTCs", (int) engine_data);
                            } else {                        //MIL Light off
                                engine_data = (can_MsgRx.data[3]);
                                sprintf(buffer,"MIL OFF, %d DTCs", (int) engine_data);
                            }
                            // Gasoline C and D bytes (can_MsgRx.data[5] and can_MsgRx.data[6])
                            //                      Test available  Test incomplete
                            // NMHC Catalyst        C0              D0
                            // NOx/SCR Monitoring   C1              D1
                            // Boost Pressure       C3              D3
                            // Exhaust Gas Sensor   C5              D5
                            // Particulate Filter   C6              D6
                            // EGR and/or VVT/VTEC  C7              D7
                        }
                        // Common Tests between Gas and Diesel Engines, byte B (can_MsgRx.data[4])
                        //                  Test available  Test incomplete
                        // Misfire          B0              B4
                        // Fuel System      B1              B5
                        // Components       B2              B6
                        break;
                    }
                    case FREEZE_DTC:                        // Locks in Diagnostic trouble Codes
                        break;
                    case FUEL_SYS_STATUS:                   // bit encoded
                        //This tells us the warmup status of the engine. Only 1 bit should be set
                        engine_data = can_MsgRx.data[3];
                        if (((int) engine_data) & 0x01) {   // Open loop - Engine warmup
                            sprintf(buffer,"Open Loop - Warmup");
                        }
                        if (((int) engine_data) & 0x02) {   // Closed Loop - O2 Sensor feedback
                            sprintf(buffer,"Closed Loop - Normal");
                        }
                        if (((int) engine_data) & 0x04) {   // Open loop,
                            sprintf(buffer,"Open Loop-Load/Decel");
                        }
                        if (((int) engine_data) & 0x08) {   // Open loop - system failure
                            sprintf(buffer,"Open Loop - FAILURE");
                        }
                        if (((int) engine_data) & 0x10) {   // Closed Loop - O2 Sensor feedback failure
                            sprintf(buffer,"Closed Loop - O2Fail");
                        }
                        if ((((int) engine_data) & 0x20) | (((int) engine_data) & 0x40) | (((int) engine_data) & 0x80)) { //These shouldnt be on, assume Proprietary status
                            sprintf(buffer,"Unsupported Status");
                        }
                        break;
                    case ENGINE_LOAD:                       // A*100/255
                        engine_data = (can_MsgRx.data[3]*100)/255;
                        sprintf(buffer,"%d %% ",(int) engine_data);
                        break;
                    case ENGINE_COOLANT_TEMP:               //     A-40              [degree C]
                        engine_data = can_MsgRx.data[3] - 40;
                        sprintf(buffer,"%d degC ",(int) engine_data);
                        break;
                    case ST_FUEL_TRIM_1:                    // (A-128)*100/128
                        engine_data = ((can_MsgRx.data[3]-128)*(100/128));
                        sprintf(buffer,"%d %% ", (int) engine_data);
                        break;
                    case LT_FUEL_TRIM_1:                    // (A-128)*100/128
                        engine_data = ((can_MsgRx.data[3]-128)*(100/128));
                        sprintf(buffer,"%d %% ", (int) engine_data);
                        break;
                    case ST_FUEL_TRIM_2:                    // (A-128)*100/128
                        engine_data = ((can_MsgRx.data[3]-128)*(100/128));
                        sprintf(buffer,"%d %% ", (int) engine_data);
                        break;
                    case LT_FUEL_TRIM_2:                    // (A-128)*100/128
                        engine_data = ((can_MsgRx.data[3]-128)*(100/128));
                        sprintf(buffer,"%d %% ", (int) engine_data);
                        break;
                    case FUEL_PRESSURE:                     // A*3
                        engine_data = (can_MsgRx.data[3]*3);
                        sprintf(buffer,"%d kPa",(int) engine_data);
                        break;
                    case INTAKE_PRESSURE:                   // A
                        engine_data = can_MsgRx.data[3];
                        sprintf(buffer,"%d kPa",(int) engine_data);
                        break;
                    case ENGINE_RPM:                        //   ((A*256)+B)/4    [RPM]
                        engine_data = ((can_MsgRx.data[3]*256) + can_MsgRx.data[4])/4;
                        sprintf(buffer,"%d rpm ",(int) engine_data);
                        break;
                    case VEHICLE_SPEED:                     // A                  [km]
                        engine_data = can_MsgRx.data[3];
                        sprintf(buffer,"%d km ",(int) engine_data);
                        break;
                    case TIMING_ADVANCE:                    // A/2 - 64
                        engine_data = (can_MsgRx.data[3]/2) - 64;
                        sprintf(buffer,"%d Deg",(int) engine_data);
                        break;
                    case INTAKE_TEMP:                       // A - 40
                        engine_data = (can_MsgRx.data[3] - 40);
                        sprintf(buffer,"%d DegC",(int) engine_data);
                        break;
                    case MAF_SENSOR:                        // ((256*A)+B) / 100  [g/s]
                        engine_data = ((can_MsgRx.data[3]*256) + can_MsgRx.data[4])/100;
                        sprintf(buffer,"%d g/s",(int) engine_data);
                        break;
                    case THROTTLE:                          // A*100/255
                        engine_data = (can_MsgRx.data[3]*100)/255;
                        sprintf(buffer,"%d %% ",(int) engine_data);
                        break;
                    case COMMANDED_SEC_AIR:                 // bit encoded
                        engine_data = can_MsgRx.data[3];
                        if (((int) engine_data) & 0x01) {           //Upstream of Catalytic Converter
                            sprintf(buffer,"Upstream of Cat.");
                        }
                        if (((int) engine_data) & 0x02) {           //Downstream of Catalytic Converter
                            sprintf(buffer,"Downstream of Cat.");
                        }
                        if (((int) engine_data) & 0x04) {           //From outside atmosphere or off
                            sprintf(buffer,"Off");
                        }
                        break;
                    case O2_SENS_PRES: {                    // A [A0..A3] == Bank 1, [A4..A7] == Bank 2
                        engine_data = (can_MsgRx.data[3]);  //Check # of O2 sensors present by masking individual bits and counting
                        int o2pres = 0;
                        if (((int) engine_data) & 0x01) {           // Bank 1 Sensor 1
                            o2pres++;
                        }
                        if (((int) engine_data) & 0x02) {           // Bank 1 Sensor 2
                            o2pres++;
                        }
                        if (((int) engine_data) & 0x04) {           // Bank 1 Sensor 3
                            o2pres++;
                        }
                        if (((int) engine_data) & 0x08) {           // Bank 1 Sensor 4
                            o2pres++;
                        }
                        if (((int) engine_data) & 0x10) {           // Bank 2 Sensor 1
                            o2pres++;
                        }
                        if (((int) engine_data) & 0x20) {           // Bank 2 Sensor 2
                            o2pres++;
                        }
                        if (((int) engine_data) & 0x40) {           // Bank 2 Sensor 3
                            o2pres++;
                        }
                        if (((int) engine_data) & 0x80) {           // Bank 2 Sensor 4
                            o2pres++;
                        }
                        sprintf(buffer,"%d Present",(int) o2pres);
                        break;
                    }
                    case O2_B1S1_VOLTAGE:                   // A/200, (B-128) * 100/128
                        engine_data = (can_MsgRx.data[3]/200);
                        sprintf(buffer,"%d V ",(int) engine_data);              //Raw O2 Voltage
                        if (can_MsgRx.data[4] & 0xFF) {
                            sprintf(buffer,"Not Present");
                        } else {
                            engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                            sprintf(buffer2,"%d %% ",(int) engine_data);        //Calculated lean/rich
                        }
                        break;
                    case O2_B1S2_VOLTAGE:                   //
                        engine_data = (can_MsgRx.data[3]/200);
                        sprintf(buffer,"%d V ",(int) engine_data);
                        if (can_MsgRx.data[4] & 0xFF) {
                            sprintf(buffer,"Not Present");
                        } else {
                            engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                            sprintf(buffer2,"%d %% ",(int) engine_data);
                        }
                        break;
                    case O2_B1S3_VOLTAGE:                   //
                        engine_data = (can_MsgRx.data[3]/200);
                        sprintf(buffer,"%d V ",(int) engine_data);
                        if (can_MsgRx.data[4] & 0xFF) {
                            sprintf(buffer,"Not Present");
                        } else {
                            engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                            sprintf(buffer2,"%d %% ",(int) engine_data);
                        }
                        break;
                    case O2_B1S4_VOLTAGE:                   //
                        engine_data = (can_MsgRx.data[3]/200);
                        sprintf(buffer,"%d V ",(int) engine_data);
                        if (can_MsgRx.data[4] & 0xFF) {
                            sprintf(buffer,"Not Present");
                        } else {
                            engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                            sprintf(buffer2,"%d %% ",(int) engine_data);
                        }
                        break;
                    case O2_B2S1_VOLTAGE:                   //
                        engine_data = (can_MsgRx.data[3]/200);
                        sprintf(buffer,"%d V ",(int) engine_data);
                        if (can_MsgRx.data[4] & 0xFF) {
                            sprintf(buffer,"Not Present");
                        } else {
                            engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                            sprintf(buffer2,"%d %% ",(int) engine_data);
                        }
                        break;
                    case O2_B2S2_VOLTAGE:                   //
                        engine_data = (can_MsgRx.data[3]/200);
                        sprintf(buffer,"%d V ",(int) engine_data);
                        if (can_MsgRx.data[4] & 0xFF) {
                            sprintf(buffer,"Not Present");
                        } else {
                            engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                            sprintf(buffer2,"%d %% ",(int) engine_data);
                        }
                        break;
                    case O2_B2S3_VOLTAGE: {                 //
                        engine_data = (can_MsgRx.data[3]/200);
                        sprintf(buffer,"%d V ",(int) engine_data);
                        if (can_MsgRx.data[4] & 0xFF) {
                            sprintf(buffer,"Not Present");
                        } else {
                            engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                            sprintf(buffer2,"%d %% ",(int) engine_data);
                        }
                        break;
                    }
                    case O2_B2S4_VOLTAGE: {                 //
                        engine_data = (can_MsgRx.data[3]/200);
                        sprintf(buffer,"%d V ",(int) engine_data);
                        if (can_MsgRx.data[4] & 0xFF) {
                            sprintf(buffer,"Not Present");
                        } else {
                            engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                            sprintf(buffer2,"%d %% ",(int) engine_data);
                        }
                        break;
                    }
                    case OBDII_STANDARDS: {                 //bit encoded NOT DONE
                        engine_data = can_MsgRx.data[3];
                        if (((int) engine_data) & 0x0D) {           //JOBD, EOBD, and OBD II
                            sprintf(buffer,"JOBD,EOBD,OBDII");
                        }
                        if (((int) engine_data) & 0x0C) {           //JOBD and EOBD
                            sprintf(buffer,"JOBD,EOBD");
                        }
                        if (((int) engine_data) & 0x0B) {           //JOBD and OBDII
                            sprintf(buffer,"JOBD,OBDII");
                        }
                        if (((int) engine_data) & 0x0A) {           //JOBD
                            sprintf(buffer,"JOBD");
                        }
                        if (((int) engine_data) & 0x09) {           //EOBD, OBD, and OBD II
                            sprintf(buffer,"EOBD,OBDI,OBDII");
                        }
                        if (((int) engine_data) & 0x08) {           //EOBD and OBD
                            sprintf(buffer,"EOBD,OBDI");
                        }
                        if (((int) engine_data) & 0x07) {           //EOBD and OBDII
                            sprintf(buffer,"EOBD,OBDII");
                        }
                        if (((int) engine_data) & 0x06) {           //EOBD
                            sprintf(buffer,"EOBD");
                        }
                        if (((int) engine_data) & 0x05) {           //Not meant to comply with any OBD standard
                            sprintf(buffer,"No Compliance");
                        }
                        if (((int) engine_data) & 0x04) {           //OBDI
                            sprintf(buffer,"OBDI");
                        }
                        if (((int) engine_data) & 0x03) {           //OBD and OBDII
                            sprintf(buffer,"OBDI,OBDII");
                        }
                        if (((int) engine_data) & 0x02) {           //OBD and defined by the EPA
                            sprintf(buffer,"OBD");
                        }
                        if (((int) engine_data) & 0x01) {           //OBD-II as defined by CARB
                            sprintf(buffer,"OBDII");
                        }
                        sprintf(buffer,"ERROR");
                        break;
                    }
                    case O2_SENS_PRES_ALT: {                //*******************
                        engine_data = (can_MsgRx.data[3]);  //Check # of O2 sensors present by masking individual bits and counting
                        int o2presalt = 0;
                        if (((int) engine_data) & 0x01) {           // Bank 1 Sensor 1
                            o2presalt++;
                        }
                        if (((int) engine_data) & 0x02) {           // Bank 1 Sensor 2
                            o2presalt++;
                        }
                        if (((int) engine_data) & 0x04) {           // Bank 2 Sensor 1
                            o2presalt++;
                        }
                        if (((int) engine_data) & 0x08) {           // Bank 2 Sensor 2
                            o2presalt++;
                        }
                        if (((int) engine_data) & 0x10) {           // Bank 3 Sensor 1
                            o2presalt++;
                        }
                        if (((int) engine_data) & 0x20) {           // Bank 3 Sensor 2
                            o2presalt++;
                        }
                        if (((int) engine_data) & 0x40) {           // Bank 4 Sensor 1
                            o2presalt++;
                        }
                        if (((int) engine_data) & 0x80) {           // Bank 4 Sensor 2
                            o2presalt++;
                        }
                        sprintf(buffer,"%d Present",(int) o2presalt);
                        break;
                    }
                    case AUX_IN_STATUS: {                   // A (A0 == PTO Active)
                        engine_data = can_MsgRx.data[3];
                        if (((int) engine_data) & 0x01) {
                            sprintf(buffer,"PTO Active");
                        } else {
                            sprintf(buffer,"PTO Inactive");
                        }
                        break;
                    }
                    case ENGINE_RUNTIME:                    // (A*256)+B
                        engine_data = (can_MsgRx.data[3]*256)+(can_MsgRx.data[4]);
                        sprintf(buffer,"%d Sec",(int) engine_data);
                        break;
                    case PID_21_40:                         // bit encoded NOT DONE
                        PID2140 = ((can_MsgRx.data[3] << 24) | (can_MsgRx.data[4] << 16) | (can_MsgRx.data[5] << 8) | (can_MsgRx.data[6]));
                        break;
                    case DIST_TRAVELED_MIL:                 // (A*256) + B
                        engine_data = ((can_MsgRx.data[3] * 256) + can_MsgRx.data[4]);
                        sprintf(buffer,"%d km",(int) engine_data);
                        break;
                    case FUEL_RAIL_PRESSURE:                // ((A*256)+B)*0.079
                        engine_data = ((can_MsgRx.data[3] * 256)+can_MsgRx.data[4])*0.079;
                        sprintf(buffer,"%d kPa",(int) engine_data);
                        break;
                    case FUEL_RAIL_PRES_ALT:                // ((A*256)+B)*0.079
                        engine_data = ((can_MsgRx.data[3] * 256) + can_MsgRx.data[4])*10;
                        sprintf(buffer,"%d kPa",(int) engine_data);
                        break;
                    case O2S1_WR_LAMBDA_V:                  // ((A*256)+B)*2/65535 [ratio], ((C*256)+D)*8/65535 [V]
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*2)/65535);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])*8)/65535);
                        sprintf(buffer2,"%d V",(int) engine_data);
                        break;
                    case O2S2_WR_LAMBDA_V:                  //
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*2)/65535);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])*8)/65535);
                        sprintf(buffer2,"%d V",(int) engine_data);
                        break;
                    case O2S3_WR_LAMBDA_V:                  //
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*2)/65535);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])*8)/65535);
                        sprintf(buffer2,"%d V",(int) engine_data);
                        break;
                    case O2S4_WR_LAMBDA_V:                  //
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*2)/65535);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])*8)/65535);
                        sprintf(buffer2,"%d V",(int) engine_data);
                        break;
                    case O2S5_WR_LAMBDA_V:                  //
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*2)/65535);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])*8)/65535);
                        sprintf(buffer2,"%d V",(int) engine_data);
                        break;
                    case O2S6_WR_LAMBDA_V:                  //
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*2)/65535);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])*8)/65535);
                        sprintf(buffer2,"%d V",(int) engine_data);
                        break;
                    case O2S7_WR_LAMBDA_V:                  //
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*2)/65535);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])*8)/65535);
                        sprintf(buffer2,"%d V",(int) engine_data);
                        break;
                    case O2S8_WR_LAMBDA_V:                  //
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*2)/65535);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])*8)/65535);
                        sprintf(buffer2,"%d V",(int) engine_data);
                        break;
                    case COMMANDED_EGR:                     // 100*A/255
                        engine_data = (can_MsgRx.data[3]*100/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case EGR_ERROR:                         // (A-128)*100/128
                        engine_data = ((can_MsgRx.data[3]-128)*(100/128));
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case COMMANDED_EVAP_P:                  // 100*A/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case FUEL_LEVEL:                        //100*A/255
                        engine_data = ((100*can_MsgRx.data[3])/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case WARMUPS_SINCE_CLR:                 //A
                        engine_data = (can_MsgRx.data[3]);
                        sprintf(buffer,"%d Warmups",(int) engine_data);
                        break;
                    case DIST_SINCE_CLR:                    //A*256+B   [km]
                        engine_data = ((can_MsgRx.data[3]*256)+can_MsgRx.data[4]);
                        sprintf(buffer,"%d km",(int) engine_data);
                        break;
                    case EVAP_PRESSURE:                     //((A*256)+B)/4
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/4);
                        sprintf(buffer,"%d Pa",(int) engine_data); //Yes it's in pascals
                        break;
                    case BAROMETRIC_PRESSURE:               //A
                        engine_data = can_MsgRx.data[3];
                        sprintf(buffer,"%d kPa",(int) engine_data);
                        break;
                    case O2S1_WR_LAMBDA_I:                  //((A*256)+B)/32,768 [Ratio], ((C*256)+D)/256 - 128 [mA]
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/32768);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])/256)-128);
                        sprintf(buffer2,"%d mA",(int) engine_data);
                        break;
                    case O2S2_WR_LAMBDA_I:
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/32768);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])/256)-128);
                        sprintf(buffer2,"%d mA",(int) engine_data);
                        break;
                    case O2S3_WR_LAMBDA_I:
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/32768);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])/256)-128);
                        sprintf(buffer2,"%d mA",(int) engine_data);
                        break;
                    case O2S4_WR_LAMBDA_I:
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/32768);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])/256)-128);
                        sprintf(buffer2,"%d mA",(int) engine_data);
                        break;
                    case O2S5_WR_LAMBDA_I:
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/32768);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])/256)-128);
                        sprintf(buffer2,"%d mA",(int) engine_data);
                        break;
                    case O2S6_WR_LAMBDA_I:
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/32768);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])/256)-128);
                        sprintf(buffer2,"%d mA",(int) engine_data);
                        break;
                    case O2S7_WR_LAMBDA_I:
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/32768);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])/256)-128);
                        sprintf(buffer2,"%d mA",(int) engine_data);
                        break;
                    case O2S8_WR_LAMBDA_I:
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/32768);
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = ((((can_MsgRx.data[5]*256)+can_MsgRx.data[6])/256)-128);
                        sprintf(buffer2,"%d mA",(int) engine_data);
                        break;
                    case CAT_TEMP_B1S1:                     //((A*256)+B)/10 - 40 [DegC]
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/10)-40);
                        sprintf(buffer,"%d DegC",(int) engine_data);
                        break;
                    case CAT_TEMP_B1S2:
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/10)-40);
                        sprintf(buffer,"%d DegC",(int) engine_data);
                        break;
                    case CAT_TEMP_B2S1:
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/10)-40);
                        sprintf(buffer,"%d DegC",(int) engine_data);
                        break;
                    case CAT_TEMP_B2S2:
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/10)-40);
                        sprintf(buffer,"%d DegC",(int) engine_data);
                        break;
                    case PID_41_60:                         //bit encoded NOT DONE
                        PID4160 = ((can_MsgRx.data[3] << 24) | (can_MsgRx.data[4] << 16) | (can_MsgRx.data[5] << 8) | (can_MsgRx.data[6]));
                        break;
                    case MONITOR_STATUS:                    // bit encoded
                        //LUT: (Uses multiple bytes) A7..0 always 0
                        //                  Test enabled    Test Incomplete
                        // Misfire          B0              B4
                        // Fuel System      B1              B5
                        // Components       B2              B6
                        // Reserved         B3              B7
                        // Catalyst         C0              D0
                        // Heated Catalyst  C1              D1
                        // Evap System      C2              D2
                        // Sec. Ait system  C3              D3
                        // A/C Refrigerant  C4              D4
                        // O2 Sensor        C5              D5
                        // O2 Sensor Heater C6              D6
                        // EGR System       C7              D7
                        break;
                    case ECU_VOLTAGE:                       //((A*256)+B)/1000 [V]
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/1000);
                        sprintf(buffer,"%d V",(int) engine_data);
                        break;
                    case ABSOLUTE_LOAD:                     //((A*256)+B)*100/255 [%]
                        engine_data = ((((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case COMMANDED_EQUIV_R:                 //((A*256)+B)/32768 [Ratio]
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])/32768);
                        sprintf(buffer,"Ratio %d",(int) engine_data);
                        break;
                    case REL_THROTTLE_POS:                  // A*100/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case AMB_AIR_TEMP:                      // A-40 [DegC]
                        engine_data = (can_MsgRx.data[3]-40);
                        sprintf(buffer,"%d DegC",(int) engine_data);
                        break;
                    case ABS_THROTTLE_POS_B:                // A*100/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case ABS_THROTTLE_POS_C:                // A*100/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case ACCEL_POS_D:                       // A*100/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case ACCEL_POS_E:                       // A*100/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case ACCEL_POS_F:                       // A*100/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case COMMANDED_THROTTLE:                //A*100/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case TIME_RUN_WITH_MIL:                 //(A*256)+B [minutes]
                        engine_data = ((can_MsgRx.data[3]*256)/(can_MsgRx.data[4]));
                        sprintf(buffer,"%d Mins",(int) engine_data);
                        break;
                    case TIME_SINCE_CLR:                    //(A*256)+B [minutes]
                        engine_data = ((can_MsgRx.data[3]*256)/(can_MsgRx.data[4]));
                        sprintf(buffer,"%d Mins",(int) engine_data);
                        break;
                    case MAX_R_O2_VI_PRES:                  //A,B,C,D*10 [Ratio,V,mA,kPa]
                        engine_data = can_MsgRx.data[3];
                        sprintf(buffer,"Ratio: %d",(int) engine_data);
                        engine_data = can_MsgRx.data[4];
                        sprintf(buffer,"%d V",(int) engine_data);
                        engine_data = can_MsgRx.data[5];
                        sprintf(buffer,"%d mA",(int) engine_data);
                        engine_data = (can_MsgRx.data[6]*10);
                        sprintf(buffer,"%d kPa",(int) engine_data);
                        break;
                    case MAX_AIRFLOW_MAF:                   //A*10 [g/s]
                        engine_data = (can_MsgRx.data[3]*10);
                        sprintf(buffer,"%d g/s",(int) engine_data);
                        break;
                    case FUEL_TYPE:                         // USE LUT NOT DONE
                        break;
                    case ETHANOL_PERCENT:                   //A*100/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case ABS_EVAP_SYS_PRES:                 //1/200 per bit [kPa] ----NOT DONE----
                        break;
                    case EVAP_SYS_PRES:                     // (A*256)+B - 32768 [Pa]
                        engine_data = ((can_MsgRx.data[3]*256)+can_MsgRx.data[4]-32768);
                        sprintf(buffer,"%d Pa",(int) engine_data);
                        break;
                    case ST_O2_TRIM_B1B3:                   // ((A-128)*100/128 (B-128)*100/128 [%]
                        engine_data = ((can_MsgRx.data[3]-128)*(100/128));
                        sprintf(buffer,"%d %%",(int) engine_data);
                        engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case LT_O2_TRIM_B1B3:
                        engine_data = ((can_MsgRx.data[3]-128)*(100/128));
                        sprintf(buffer,"%d %%",(int) engine_data);
                        engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case ST_02_TRIM_B2B4:
                        engine_data = ((can_MsgRx.data[3]-128)*(100/128));
                        sprintf(buffer,"%d %%",(int) engine_data);
                        engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case LT_O2_TRIM_B2B4:
                        engine_data = ((can_MsgRx.data[3]-128)*(100/128));
                        sprintf(buffer,"%d %%",(int) engine_data);
                        engine_data = ((can_MsgRx.data[4]-128)*(100/128));
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case ABS_FUEL_RAIL_PRES:                //((A*256)+B)*10 [kPa]
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*10);
                        sprintf(buffer,"%d kPa",(int) engine_data);
                        break;
                    case REL_ACCEL_POS:                     //A*100/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case HYBRID_BATT_PCT:                   //A*100/255 [%]
                        engine_data = ((can_MsgRx.data[3]*100)/255);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case ENGINE_OIL_TEMP:                   //A-40 [DegC]
                        engine_data = (can_MsgRx.data[3]-40);
                        sprintf(buffer,"%d DegC",(int) engine_data);
                        break;
                    case FUEL_TIMING:                       //(38655-((A*256)+B))/128
                        engine_data = ((38655 - ((can_MsgRx.data[3]*256)+can_MsgRx.data[4]))/128);
                        sprintf(buffer,"%d Deg",(int) engine_data);
                        break;
                    case FUEL_RATE:                         //((A*256)+B)*0.05
                        engine_data = (((can_MsgRx.data[3]*256)+can_MsgRx.data[4])*0.05);
                        sprintf(buffer,"%d L/m",(int) engine_data);
                        break;
                    case EMISSIONS_STANDARD:                //bit encoded ----NOT DONE----
                        break;
                    case DEMANDED_TORQUE:                   //A-125 [%]
                        engine_data = (can_MsgRx.data[3]-125);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case ACTUAL_TORQUE:                     //A-125 [%]
                        engine_data = (can_MsgRx.data[3]-125);
                        sprintf(buffer,"%d %%",(int) engine_data);
                        break;
                    case REFERENCE_TORQUE:                  //A*256+b [Nm]
                        engine_data = ((can_MsgRx.data[3]*256)+can_MsgRx.data[4]);
                        sprintf(buffer,"%d Nm",(int) engine_data);
                        break;
                    case ENGINE_PCT_TORQUE:                 //A-125 idle, B-125 pt 1, C-125, D-125
                        engine_data = (can_MsgRx.data[3]);
                        sprintf(buffer,"%d %% - Idle",(int) engine_data);
                        engine_data = (can_MsgRx.data[4]);
                        sprintf(buffer2,"%d %% - Point 1",(int) engine_data);
                        engine_data = (can_MsgRx.data[5]);
                        sprintf(buffer3,"%d %% - Point 2",(int) engine_data);
                        engine_data = (can_MsgRx.data[6]);
                        sprintf(buffer4,"%d %% - Point 3",(int) engine_data);
                        break;
                    case AUX_IO_SUPPORTED:                  //Bit encoded ----NOT DONE----
                        break;
                    case P_MAF_SENSOR:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_ENGINE_COOLANT_T:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_INTAKE_TEMP:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_COMMANDED_EGR:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_COMMANDED_INTAKE:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_EGR_TEMP:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_COMMANDED_THROT:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_FUEL_PRESSURE:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_FUEL_INJ_PRES:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_TURBO_PRESSURE:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_BOOST_PRES_CONT:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_VGT_CONTROL:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_WASTEGATE_CONT:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_EXHAUST_PRESSURE:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_TURBO_RPM:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_TURBO_TEMP1:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_TURBO_TEMP2:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_CACT:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_EGT_B1:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_EGT_B2:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_DPF1:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_DPF2:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_DPF_TEMP:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_NOX_NTE_STATUS:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_PM_NTE_STATUS:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_ENGINE_RUNTUME:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_ENGINE_AECD_1:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_ENGINE_AECD_2:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_NOX_SENSOR:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_MANIFOLD_TEMP:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_NOX_SYSTEM:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_PM_SENSOR:
                        sprintf(buffer,"Not supported");
                        break;
                    case P_IN_MANIF_TEMP:
                        sprintf(buffer,"Not supported");
                        break;
                }
                lcd.locate(0,1);
                lcd.printf("%s", buffer);
                if (buffer2 != NULL && buffer2[0] == '\0') lcd.printf(" , %s", buffer2);
                if (buffer3 != NULL && buffer3[0] == '\0') lcd.printf(" , %s", buffer3);
                if (buffer4 != NULL && buffer4[0] == '\0') lcd.printf(" , %s", buffer4);
                return 1;
            }
        }
    }    
    lcd.printf("CANBus Timeout -- ");
    pc.printf("CANBus Timeout -- ");
    return 0;
}
} // namespace mbed 