/*  
 *                ------ General PIDs -------- 
 *  
 *  This sketch shows how to use the general function CiARequest and the 
 *  standard OBD-II PIDs codes to get data from the vehicle. This codes 
 *  are used to request data from a vehicle, used as a diagnostic tool.
 *
 *  Copyright (C) 2014 Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify  
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation, either version 3 of the License, or  
 *  (at your option) any later version.  
 *   
 *  This program is distributed in the hope that it will be useful,  
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of  
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
 *  GNU General Public License for more details.  
 *   
 *  You should have received a copy of the GNU General Public License  
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
 *
 *  Version:          0.1 
 *  Implementation:   Luis Antonio Martin & Ahmad Saad
 */

// Include always these libraries before using the CAN BUS functions
#include "CAN.h"
#include "arduPi.h"

#define myPID 0x00 // <------- Define here your PID

// Create an instance of the object
CAN myCAN = CAN();

void setup()
{ 	
  // Turn on the Serial 
  Serial.begin(115200);
  delay(100);

  // Print initial message 
  printf("Initializing CAN Bus...\n");	

  // Configuring the BUS at 500 Kbit/s
  myCAN.begin(500);   
  printf("CAN Bus initialized at 500 KBits/s\n");  
}

void loop()
{
  int data;

  // Read the value of RPM if the engine 
  myCAN.CiARequest(myPID);


  if (myCAN.messageRx.id == ID_RESPONSE) 
  {
    data = uint16_t(myCAN.messageRx.data[3]) ; // <-------- Insert the formula here
    myCAN.printMessage(&myCAN.messageRx);
  }

  // Print received data in the serial monitor
  printf("Returned data : %d",data); 
  printf("\n");
  delay(1000); 

}

int main (){
	setup();
	while(1){
		loop();
	}
	return (0);
}
