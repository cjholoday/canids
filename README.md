# README #

### What is this repository for? ###

* The attack, detection, and learning phases for the MDP IDS

### Contribution guidelines ###

* Make more comments so that others can understand!
* The commit name should be as informative as possible!

### Attack directory ###

* Currently WIP
* Should send CAN packets at high frequencies
* Process for sending packets TBD since RbPi does not have built in joystick like mbed did

### Reading Can Messages and Frequency, Phase 1 ###

### Detection Directory ###

* WIP
* Learning phase collects CAN packet arrival times and determines acceptable frequency per packet
* Detection phase analyzes incoming packets and determines if they are valid based on learning results