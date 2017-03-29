# Reads and prints messages on the CAN bus to the terminal
# Includes time stamp, message ID, data length code, message content

import can
can.rc['interface'] = 'socketcan_native'
# can.rc['interface'] = 'socketcan_ctypes'
# socketcan_ctypes should be used for older Linux kernels though
#   both native and ctypes seem to work on Python 3.3+
can.rc['channel'] = 'vcan0'
from can.interfaces.interface import Bus

bus = Bus()
while(1):
    print(bus.recv())
