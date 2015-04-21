# README #

This README would normally document whatever steps are necessary to get your application up and running.

### What is this repository for? ###

* ecu_reader
* Version 1.0

### Contribution guidelines ###

* Make more comments so that others can understand!
* The commit name should be as informative as possible!

### Who do I talk to? ###

* This is a private project
* So if you are not from the group and you are in, 
* please email dangying@umich.edu to tell me the project is accidentally set public

### To run the test files ###

* Joystick down to run the current attacks
* Once "test attacks to run" is shown, you can begin sending attack messages
* Down: force headlights off
* Up: force headlights on
* Right: Fan to max
* Left: Fan to min

### Reading Can Messages and Frequency, Phase 1 ###

* The reading Can Messages and Frequency program is for the car/automaker to do once and store the data. 
* We are storing it in a .txt file.
* The .txt file composes the CAN ID and the frequency of the CAN ID over a user defined period of time.

### Detection and Alerts, Phase 2 ###

* We are creating a hash table to temporarily store the information from phase 1.
* We compare the frequencies from the hash table and the current frequencies being read in. 
* Then decide whether to send an alert that the system is hacked or not.