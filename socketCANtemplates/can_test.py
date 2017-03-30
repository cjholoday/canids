# Use Python 3.2+
# Reads and prints messages on the CAN bus to the terminal
#   Includes time stamp, message ID, data length code, message content
# Stores incoming message IDs and data in a list

NUMMESSAGES = 5 # Will read in NUMMESSAGES messages

import can
import array
from can.interfaces.interface import Bus
can.rc['interface'] = 'socketcan_native'
# can.rc['interface'] = 'socketcan_ctypes'
# socketcan_ctypes should be used for older Linux kernels though
#   both native and ctypes seem to work on Python 3.3+
#can.rc['channel'] = 'vcan0'

bus1 = Bus(channel='vcan0')
listener1 = can.BufferedReader()
notifier1 = can.Notifier(bus1,[listener1])
# Puts messages in a queue (FIFO)

messagesDec = []
messagesBin = []

for i in range(0,NUMMESSAGES):
    thisMessage = listener1.get_message(10)
    print("This is the message: ", thisMessage)
    print("This is the message ID: ", thisMessage.arbitration_id) # Returns decimal value
    print("This is the data content: ", thisMessage.data)
    # Above is in hex, converts to printable char when possible
    thisMessage.dataDec = int.from_bytes(thisMessage.data, byteorder='big')
    # Above is where Python 3.2+ needed, use struct module in lower versions
    print("This is the data content in decimal: ", thisMessage.dataDec)
    thisMessage.dataBin = bin(thisMessage.dataDec)
    thisMessage.arbitration_idBin = bin(thisMessage.arbitration_id)
    print("This is the data content in binary: ", bin(thisMessage.dataDec)) # Leading 0s cut
    messagesDec.append([thisMessage.arbitration_id, thisMessage.dataDec])
    messagesBin.append([thisMessage.arbitration_idBin, thisMessage.dataBin])
    # input() # Press enter to continue

print(messagesDec)
print(messagesBin)

"""
while(1):
    bus.flush_tx_buffer()
    message = bus.recv()
    print(message.dlc, message.data)
"""
