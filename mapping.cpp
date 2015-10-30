#include "learning.h"
#include "mbed.h"
#include "ecu_reader.h"
#include "globals.h"
#include "TextLCD.h"
#include "GPS.h"
#include "SDFileSystem.h"
#include <errno.h> //system error numbers
#include <fstream>
#include <unordered_map>
#include <string>

using namespace std;

//for unordered_map
int main(){
	ifstream file;
	double frequency;
	string identifierCAN;
	unordered_map<string, double> map{,0};
	file.open("/sd/messagestore.txt");
	
	if(!file.open("/sd/messagestore.txt")){
		lcd.printf("file open failed %d", errno);
		exit(1);
	}

	while (file >> identifierCAN) {
		file >> frequency;
		map.insert({identifierCAN, frequency});	
	}
	
	file.close();
	
	return 0;
}
