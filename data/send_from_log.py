import log_utils
from timeit import default_timer
import sys
import can
from can.interfaces.interface import Bus

#python3 send_from_log.py [device name] [logfile]

can.rc['interface'] = 'socketcan_ctypes'
can.rc['channel'] = 'can0'
bus = Bus()

chan_in = sys.argv[1]
LOGFILE = sys.argv[2]
#ID = int(sys.argv[2])

can.rc['interface'] = 'socketcan_ctypes'
can.rc['channel'] = chan_in
bus = Bus()

def send_message(message):
    soc_message = can.Message(extended_id = False, is_error_frame = False, arbitration_id = message.can_id, data = message.data)
    bus.send(soc_message)

def message_alter(message, altered_bytes):
    for i in range(len(message.data)):
        if i in altered_bytes:
            message.data[i]=0

if __name__ == '__main__':

    messages = log_utils.return_message_list(LOGFILE)
    IDmesgs = []
    
    #altered_bytes = {0,1,2,7,8}

    for msg in messages:
       #continue
       #message_alter(msg, altered_bytes)
        IDmesgs.append(msg)

    start = default_timer()

    while 1:
        for temp in messages:
            while round(default_timer()-start, 2) != round(temp.time, 2):
                continue
            #temp.print_Message()
            send_message(temp)
    
