from time import sleep
import can
from can.interfaces.interface import Bus
import sys

id_in = int(sys.argv[2])
channel_in = sys.argv[1]

can.rc['interface'] = 'socketcan_ctypes'
can.rc['channel'] = channel_in
bus = Bus()

if __name__ == '__main__':
    message = can.Message(extended_id = False, is_error_frame = False, arbitration_id = id_in, data = [0x00])
    while 1:
        bus.send(message)