import can
from can.interfaces.interface import Bus
from can import Message
from can import Logger
can.rc['interface'] = 'socketcan_native'

def sortID(thisID, arrayIn): # Sorts ID list in ascending order
    if len(arrayIn) < 1:
        thisID = [thisID, 1]
        arrayIn.append(thisID)
        return arrayIn
    for i in range(0,len(arrayIn)):
        if thisID < arrayIn[i][0]:
            thisID = [thisID, 1]
            arrayIn.insert(i,thisID)
            return arrayIn
        elif thisID == arrayIn[i][0]:
            arrayIn[i][1] += 1
            return arrayIn
        else:
            continue
    thisID = [thisID, 1]
    arrayIn.append(thisID)
    return arrayIn

idList = []

bus1 = Bus(channel='vcan0')
listener1 = can.BufferedReader()
notifier1 = can.Notifier(bus1,[listener1])

try:
    while 1:
        thisMessage = listener1.get_message(0.5)
        if thisMessage is not None:
            tempID = thisMessage.arbitration_id
            idList = sortID(tempID, idList)
except KeyboardInterrupt: # Press Ctrl-C to stop collecting data
    pass

# Will write frequency distribution to this file
filename = 'idFreqDist.csv'
outFile = open(filename, 'w')
for item in idList:
    outFile.write("%s%s%s\n" % (item[0], ',', item[1]))
outFile.close()
