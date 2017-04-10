# Assumes consistent file format

import time

inFileName = 'recording.trc'
firstDataLine = 16 # First line in file with frame data - 1
outFileName = 'msgLog' + str(time.time()) + '.csv'


inFile = open(inFileName, 'r')
fromFile = [line.split(' ') for line in inFile]
inFile.close()

data = []

for i in range(firstDataLine,len(fromFile)): # Clear all the spaces
    temp = []
    for j in range(0, len(fromFile[i])):
        if fromFile[i][j] != '':
            temp.append(fromFile[i][j])
    data.append(temp)

outputData = []

for packet in data:
    temp = [packet[1], packet[3], packet[-9:-1]]
    outputData.append(temp)

outFile = open(outFileName, 'w')
for packet in outputData:
    outFile.write('%s%s%s%s%s\n' % (packet[0], ',', packet[1], ',',
            packet[2][0] + packet[2][1] + packet[2][2] + packet[2][3] +
            packet[2][4] + packet[2][5] + packet[2][6] + packet[2][7]))
outFile.close()
