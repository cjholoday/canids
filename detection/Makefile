# Eric Hwang - 3/29/2016
# A simple makefile to run the detection code.
# Compiles learning.*, main.cpp, globals.*, detect.*

# CCDIR: Environment variable that contains path to cross compiler
CXX = $(CCDIR)/arm-linux-gnueabihf-g++

# Default flags
CXXFLAGS = -std=gnu++11

all: execute

execute: detect.o main.o learning.o globals.o
	$(CXX) $(CXXFLAGS) detect.o main.o learning.o globals.o -o execute

detect.o: detect.cpp detect.h globals.h

main.o:	main.cpp globals.h detect.h

learning.o: learning.cpp globals.h learning.h

globals.o: globals.cpp globals.h

clean:
	rm *o execute

