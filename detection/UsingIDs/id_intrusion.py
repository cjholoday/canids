import can
import struct
from can.interfaces.interface import Bus
from can import Message
from can import Logger
can.rc['interface'] = 'socketcan_native'
# can.rc['interface'] = 'socketcan_ctypes'
# socketcan_ctypes should be used for older Linux kernels though
#   both native and ctypes seem to work on Python 3.3+

#can.rc['channel'] = 'vcan0'
bus = Bus(channel='vcan0')
valid_ids = [False] * 2048

#istener = can.BufferedReader()
#notifier1 = can.Notifier(bus1,[listener])

#while (1):
    #message = listener.get_message()
    #print(message.arbitration_id)

count = 0
while (count < 10000):
    message = bus.recv(20)
    if valid_ids[message.arbitration_id] == False:
        valid_ids[message.arbitration_id] = True
        count = 0
    else:
        count += 1

print("finished reading in valid ids")

while (1):
    try:
        message = bus.recv(20)
        if valid_ids[message.arbitration_id] == False:
            print("invalid ID")
    except(KeyboardInterrupt):
        break
        
