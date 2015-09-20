#include "learning.h"
#include "detect.h"
#include "mbed.h"
#include "ecu_reader.h"
#include "globals.h"
#include "TextLCD.h"
#include "GPS.h"
#include "SDFileSystem.h"
#include <errno.h> //system error numbers

#define CANDUMP_PATH "/sd/messagestore.txt"
#define ARRAY_SIZE 0x7FF // 2048 bits for all common hexcode
#define COLLECTION_TIME_MS 5000

//for unordered_map
using namespace std;

TextLCD lcd(p18, p19, p20, p17, p16, p15, p14); // rs, rw, e, d0, d1, d2, d3
SDFileSystem sd(p5, p6, p7, p13, "sd");

Serial pc(USBTX, USBRX);

ecu_reader obdii(CANSPEED_500);     //Create object and set CAN speed

int main() {

    pc.baud(115200);

    detectFrequencies();
}