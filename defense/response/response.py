from time import sleep
import can
from can.interfaces.interface import Bus
import sys
from signal import *
from random import uniform

#"can1", bitrate_int, "receive", ID, length, data]

channel_in = sys.argv[1]
bitrate = int(sys.argv[2])
send_recv = sys.argv[3]
id_in = int(sys.argv[4])
length = int(sys.argv[5])
data = int(sys.argv[6])

can.rc['interface'] = 'socketcan_ctypes'
can.rc['channel'] = channel_in
bus = Bus()

out = [channel_in, " | ", send_recv, " | ", "bitrate: ", str(bitrate), " | ", "count: ", "" ]

def clean(*args):
    print(''.join(out))
    sys.exit(0)

if __name__ == '__main__':

    signal(SIGTERM, clean)

    count = 0

    if(send_recv == "send"):
        message = can.Message(extended_id = False, is_error_frame = False, arbitration_id = id_in, data = [data]*length)
        while 1:
            sleep(uniform(0.001,0.1))
            bus.send(message)
            count += 1
            out[8] = str(count)

    elif(send_recv == "receive"):
        print()
        while 1:
            print(bus.recv())
            count += 1
            out[8] = str(count)
~                                 
