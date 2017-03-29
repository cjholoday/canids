# Reads and prints messages on the CAN bus to the terminal
# Includes time stamp, message ID, data length code, message content

import can
can.rc['interface'] = 'socketcan_native'
# can.rc['interface'] = 'socketcan_ctypes'
# socketcan_ctypes should be used for older Linux kernels though
#   both native and ctypes seem to work on Python 3.3+
#can.rc['channel'] = 'vcan0'
from can.interfaces.interface import Bus

bus1 = Bus(channel='vcan0')
listener1 = can.BufferedReader()
notifier1 = can.Notifier(bus1,[listener1])

while(1):
    print("This is the message: ", listener1.get_message(10))
    # Puts message in a queue
    input()

"""
while(1):
    bus.flush_tx_buffer()
    message = bus.recv()
    print(message.dlc, message.data)
"""
