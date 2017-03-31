# Reads and prints messages on the CAN bus to the terminal
#   Includes time stamp, message ID, data length code, message content
# Stores NUMMESSAGES incoming message IDs and data in a list
# Floods bus with NUMMESSAGES messages of ID 0 afterwards
# Writes all received messages to messages.csv

NUMMESSAGES = 5 # Will read in NUMMESSAGES messages

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

bus1 = Bus(channel='vcan0')
listener1 = can.BufferedReader() # Queues messages
ascWriter1 = can.ASCWriter('messages.asc') # Writes messages to asc file
csvWriter1 = can.CSVWriter('messages.csv') # Writes messages to csv file
notifier1 = can.Notifier(bus1,[listener1])
# Above alternates between which listener to send messages to

messagesDec = []
messagesBin = []

for i in range(0,NUMMESSAGES):
    thisMessage = listener1.get_message(10)
    ascWriter1(thisMessage)
    csvWriter1(thisMessage)
    # Data section in CSV file is in base 64
    print("This is the message: ", thisMessage)
    print("This is the message ID: ", thisMessage.arbitration_id) # Returns decimal value
    print("This is the data content: ", thisMessage.data)
    # Above is in hex, converts to printable char when possible
    thisMessage.dataDec = int.from_bytes(thisMessage.data, byteorder='big')
    # Above is where Python 3.2+ needed, use struct module in lower versions
    # thisMessage.dataDec = struct.unpack('B',thisMessage.data) working on converting to float
    print("This is the data content in decimal: ", thisMessage.dataDec)
    thisMessage.dataBin = bin(thisMessage.dataDec)
    thisMessage.arbitration_idBin = bin(thisMessage.arbitration_id)
    print("This is the data content in binary: ", bin(thisMessage.dataDec)) # Leading 0s cut
    messagesDec.append([thisMessage.arbitration_id, thisMessage.dataDec])
    messagesBin.append([thisMessage.arbitration_idBin, thisMessage.dataBin])
    # input() # Press enter to continue

print(messagesDec)
print(messagesBin)

# Floods bus with messages of ID 0
for i in range(0,NUMMESSAGES):
    testMessage = Message(data=[0x01, 2, 3, 4, 5]) # ID=0 is default
    bus1.send(testMessage)

"""
while(1):
    bus.flush_tx_buffer()
    message = bus.recv()
    print(message.dlc, message.data)
"""
