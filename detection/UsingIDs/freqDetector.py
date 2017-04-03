inFile = open('idFreqDist.csv')
freqDist = [line.split(',') for line in inFile]
inFile.close()

for item in freqDist:
    print("%s%s%s" % (item[0], ',', item[1]))
