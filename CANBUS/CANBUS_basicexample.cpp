/* 
 *        ------ CAN Bus Basic Example --------
 * 
 *  This sketch shows how to send data through CAN Bus standard.
 *  
 *  Copyright (C) 2014 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version:          0.1
 *  Implementation:   Luis Antonio Martin  & Ahmad Saad
 */

// Include always these libraries before using the CAN BUS functions
#include "CAN.h"
#include "arduPi.h"

// ID numbers
#define IDWAITED 200
#define OWNID 100

// Create an instance of the object
CAN myCAN = CAN();

// Setting up our devices and I/Os
void setup() {
  

  // Let's open the bus. Remember the input parameter:
  // 1: 1Mbps
  // 500: 500Kbps  <--- Most frecuently used
  // 250: 250Kbp
  // 125: 125Kbps
  myCAN.begin(1000);
}

void loop() {
  
  //****************************************
  // 1. Receive data
  //****************************************
  
  if (myCAN.messageAvailable() == 1) {
    // Read the last message received.
    myCAN.getMessage(&myCAN.messageRx);
    // Print in the serial monitor the received message
    myCAN.printMessage(&myCAN.messageRx);
  }
  

  //****************************************
  // 2. Send data
  //****************************************
  
  // Insert the ID in the data structure
  myCAN.messageTx.id = OWNID;
  // These fields include the data to send
  myCAN.messageTx.data[0] = 0;
  myCAN.messageTx.data[1] += 1;
  myCAN.messageTx.data[2] = 2;
  myCAN.messageTx.data[3] = 3;
  myCAN.messageTx.data[4] = 4;
  myCAN.messageTx.data[5] = 5;
  myCAN.messageTx.data[6] = 6;
  myCAN.messageTx.data[7] = 7;

  // The length of the data structure
  myCAN.messageTx.header.length = 8;
  // Send data
  myCAN.sendMessage(&myCAN.messageTx);  
  // A time delay
  delay(1000);
}

int main (){
	setup();
	while(1){
		loop();
	}
	return (0);
}
