#ifndef LEARNING_H_
#define LEARNING_H_
#include <unordered_map>
using namespace std;

//Counts number of expected frequency per CAN packet
void messageReader();

void storeMessages();
//boolean to indicate learning algorithm ran. 
bool ran_learning;

#endif // LEARNING_H_