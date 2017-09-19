import utils
import time
from timeit import default_timer
import sys


LOGFILE = sys.argv[1]
ID = int(sys.argv[2])
SAVEDBYTES = [int(sys.argv[3]), int(sys.argv[4])]

#logfile note, 100(dashboard lights), 200(speed + mile counter), 200(accelerometer)

#200 -> 4,5 == speed 6,7 ==
import can
from can.interfaces.interface import Bus
#can.rc['interface'] = 'socketcan_native' #alternative

can.rc['interface'] = 'socketcan_ctypes'
can.rc['channel'] = 'can0'
bus = Bus()

def send_message(message):
    soc_message = can.Message(extended_id = False, is_error_frame = False, arbitration_id = message.can_id, data = message.data)
    bus.send(soc_message)

def message_alter(message, savedBYTES):
    for i in range(len(message.data)):
        if not(i in savedBYTES):
            message.data[i]=0


if __name__ == '__main__':

    messages = utils.return_message_array(LOGFILE)
    IDmesgs = []

    for temp in messages:
        if temp.can_id == ID:
            #message_alter(temp, SAVEDBYTES)
            IDmesgs.append(temp)

    start = default_timer()


    for temp in messages:
        while round(default_timer()-start, 2) != round(temp.time, 2):
            continue
        temp.print_Message()
        send_message(temp)

    print('Finish')
