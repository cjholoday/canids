import time
from timeit import default_timer
import sys
import can
from can.interfaces.interface import Bus
#can.rc['interface'] = 'socketcan_native' #alternative
can.rc['interface'] = 'socketcan_ctypes'
can.rc['channel'] = 'vcan0'
bus = Bus()

#start = default_timer()
#current time = default_timer-start
#logfile note, 100(dashboard lights), 200(speed + mile counter), 200(accelerometer)
#200 -> 4,5 == speed 6,7 ==

#spoof = int(sys.argv[1])

if __name__ == '__main__':

    can_id = 200
    can_bytes = [4,5]
    can_data = [0,0,0,0,0,0,0,0]
    
    message = can.Message(extended_id = False, is_error_frame = False, arbitration_id = can_id, data = can_data)

    #can_data[4] = spoof
    #message.data = can_data
    
    #while 1:
        #bus.send(message)

    while 1:
        bus.send(message)
        #time.sleep(1e-50)
        if(can_data[5] >= 0xFF):
            #time.sleep(0.3)
            can_data[5] = 0
            if(can_data[4] >= 0xFF):
                can_data[4] = 0
            else:
                can_data[4] += 1
        else:
            can_data[5] += 1
        message.data = can_data
