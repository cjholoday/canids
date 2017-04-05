# Reads and prints messages on the CAN bus to the terminal
#   Includes time stamp, message ID, data length code, message content
# Stores NUMMESSAGES incoming message IDs and data in a list
# Floods bus with NUMMESSAGES messages of ID 0 afterwards
# Writes all received messages to msgLog.csv

import time
import can
import struct
from can.interfaces.interface import Bus
from can import Message
can.rc['interface'] = 'socketcan_native'
# can.rc['interface'] = 'socketcan_ctypes'
# socketcan_ctypes should be used for older Linux kernels though
#   both native and ctypes seem to work on Python 3.3+
#can.rc['channel'] = 'vcan0'

bus1 = Bus(channel='vcan0')
listener1 = can.BufferedReader() # Queues messages
notifier1 = can.Notifier(bus1,[listener1])
# Above alternates between which listener to send messages to

outFile = open('msgLog'+ str(time.time()) + '.csv', 'w')

try:
    while 1:
        thisMessage = listener1.get_message(0.5)
        if thisMessage is not None:
            thisMessage.dataDec = int.from_bytes(thisMessage.data, byteorder='big')
            outFile.write('%s%s%s%s%s\n' % (thisMessage.timestamp, ',', thisMessage.arbitration_id, ',', thisMessage.dataDec))
except KeyboardInterrupt: # Press Ctrl-C to stop collecting data
    pass

outFile.close()
