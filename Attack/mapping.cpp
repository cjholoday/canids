#include "learning.h"
#include "mbed.h"
#include "ecu_reader.h"
#include "globals.h"
#include "TextLCD.h"
#include "GPS.h"
#include "SDFileSystem.h"
#include <errno.h> //system error numbers
#include <fstream>
#include <string>

using namespace std;

//for unordered_map
void doMapping(){
	ifstream file;
	double frequency;
	string identifierCAN;
	unordered_map<string, double> map{,0};
// initialize the variables and the map
	file.open("/sd/messagestore.txt");
// open the message file on the sd card
	
	if(!file.open("/sd/messagestore.txt")){
		lcd.printf("file open failed %d", errno);
		exit(1);
	// if file doesn't open then print to LCD and close program
	}

	while (file >> identifierCAN) {
		file >> frequency;
		map.insert({identifierCAN, frequency});
	// insert the CAN Id and the frequency into the map from text file	
	}
	
	file.close();
	// close file
	
	return 0;
}
