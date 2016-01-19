/*  
 *  ------ CAN Bus Get Vehicle Speed -------- 
 *  
 *  This sketch shows how to get the Vehicle Speed using the standard
 *  OBD-II PIDs codes. This codes are used to request data from a
 *  vehicle, used as a diagnostic tool. 
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
 *  Implementation:   Luis Antonio Martin  & Ahmad Saad 
 */
 
// Include always these libraries before using the CAN Bus functions
#include "CAN.h"
#include "arduPi.h"


// Create an instance of the object
CAN myCAN = CAN();

void setup()
{ 	
  // Initializes the Serial  
  Serial.begin(115200);
  delay(100);
  
  // Print initial message 
  printf("Initializing CAN Bus...\n");	
  
  // Configuring the Bus at 500 Kbit/s
  myCAN.begin(500);      
  printf("CAN Bus initialized at 500 KBits/s\n");  
}

void loop()
{		
  // Read the value of vehicle speed	
  int vehicleSpeed = myCAN.getVehicleSpeed();      

  // Print received data in the serial monitor
  printf("Vehicle Speed : %d",vehicleSpeed);
  printf(" Km/h\n");

  delay(1000); 

}


int main (){
	setup();
	while(1){
		loop();
	}
	return (0);
}

