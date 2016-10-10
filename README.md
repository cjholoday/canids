# README #

### What is this repository for? ###

* The attack, detection, and learning phases for the MDP IDS

### Contribution guidelines ###

* Make more comments so that others can understand!
* The commit name should be as informative as possible!

### Attack directory ###

* Uses cansend.c to send packets
* Currently just spam messages in order to attack
* Will look into fuzzing and replay attacks after initial testing phase

### Reading Can Messages and Frequency, Phase 1 ###

### Detection Directory ###

* WIP
* Learning phase collects CAN packet arrival times and determines acceptable frequency per packet
* Detection phase analyzes incoming packets and determines if they are valid based on learning results

### A Raspberry Pi setup manual ###

* RaspberryPiSetupManual.pdf is a basic manual for how to use Susan's pi for temporary development before the new SD cards arrives
* Corrections and updates are welcomed
