from time import sleep
import can
from can.interfaces.interface import Bus
import sys
from signal import *
from random import uniform

#"can1", bitrate_int, "receive", ID, length, data]

channel_in = sys.argv[1]
bitrate = int(sys.argv[2])
#send_recv = sys.argv[3]
#id_in = int(sys.argv[4])
#length = int(sys.argv[5])
#data = int(sys.argv[6])

can.rc['interface'] = 'socketcan_ctypes'
can.rc['channel'] = channel_in
bus = Bus()

send_recv = "receive"
out = [channel_in, " | ", send_recv, " | ", "bitrate: ", str(bitrate), " | ", "count: ", "" ]

def clean(*args):
    print(''.join(out))
    sys.exit(0)

if __name__ == '__main__':

    signal(SIGTERM, clean)
    
    consecutive_messages = 1;
    last_message_id = -1;

    while 1:
        if(send_recv == "receive"):
            while 1:
                temp_msg = bus.recv();
                id = temp_msg.arbitration_id;
                if id == last_message_id:
                    consecutive_messages += 1
                else:
                    consecutive_messages = 1
                last_message_id = id
                print(consecutive_messages)
                if consecutive_messages == 10:
                    print("5 consecutive messages with same ID")
                    send_recv = "send"
                    break;
                    #consecutive_messages = 0
                #print(bus.recv())
                #count += 1
                #out[8] = str(count)    

        elif(send_recv == "send"):
            message = can.Message(extended_id = False, is_error_frame = False, arbitration_id = last_message_id, data = [0])
            while 1:
                sleep(uniform(0.001,0.1))
                bus.send(message)


