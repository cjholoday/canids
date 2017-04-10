# Assumes conversion from trc to csv

inFileName = 'msgLog1491836844.2190225.csv'
outFilename = 'msgLog-HEX-1491836844.2190225.csv'

inFile = open(inFileName)
fromFile = [line.split(',') for line in inFile]
# timestamp, network_name, msg_id#msg_data
inFile.close()

for line in fromFile:
    line[1] = '0x' + line[1]
    line[1] = int(line[1],16)
    line[2] = '0x' + line[2][:-1] # Delete newline at end
    line[2] = int(line[2],16)

outFile = open(outFilename, 'w')
for line in fromFile:
    outFile.write('%s%s%s%s%s\n' % (line[0], ',', line[1], ',', line[2]))
outFile.close()
