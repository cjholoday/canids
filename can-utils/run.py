#run.py
#python script to run a program many times
import sys
import os

if len(sys.argv) < 3:
	print "Usage: python run.py <program to run> <arg0> <arg1> ... <argn> <number of times>"
	sys.exit()
command = str(sys.argv[1])
times = int(sys.argv[len(sys.argv) - 1])
for i in range(2, len(sys.argv) - 1):
	command += ' ' + str(sys.argv[i])
for i in range(0, times):
	returnCode = os.system(command)
	if (returnCode != 0):
		break
