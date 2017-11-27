from time import sleep
import can
from can.interfaces.interface import Bus
import sys
from signal import *
from random import uniform


def response(payloads, quiet_response, channel_in):

    can.rc['interface'] = 'socketcan_ctypes'
    can.rc['channel'] = channel_in
    bus = Bus()
 
    send_recv = "receive" 


#    if(payloads != None):
#        continue
    
    consecutive_messages = 1;
    last_message_id = -1;

    mod = 1
    current = None

    while 1:
        if current != None:
            send_recv == "send"
        
        if send_recv == "receive":
            while 1:
                current = payloads.get()
                    #print(current)
                #    if current != None:
                print(current)
                #        break
                #    print(current)
                #    continue

        elif send_recv == "send":
            message = can.Message(extended_id = False, is_error_frame = False, arbitration_id = current.arbitration_id, data = [0])
            count = 0
            while 1:
                bus.send(message)
                count += 1
                if (count%mod)==0:
                    send_recv = "receive"
                    mod *= 2
                    current = None
                    break
 #if this repsponse succeeds, then there needs to be a detection of success
 #this would then stop the response, and move back to send_recv = "receive"

if __name__ == '__main__':
    response(None, None, None)


