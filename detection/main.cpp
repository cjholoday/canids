#include "globals.h"
#include "detect.h"
#include "learning.h"
#include "stdio.h"

int main() {

    printf("Automotive IDS initializing...");
    int msgReaderErr = messageReader();
    switch (msgReaderErr){
	    case 0:
	    break;
	    case 1:
	    //can read error
	    break;
	    case 2:
	    //file open error
	    break;
	    default:
	    break;
    }


    return 0;
}
