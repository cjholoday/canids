#ifndef LEARNING_H_
#define LEARNING_H_
#include <unordered_map>
using namespace std;

struct frequency_struct {
    double correct_frequency;
    double current_frequency;
};

//Counts number of expected frequency per CAN packet
void messageReader();

void storeMessages();
//boolean to indicate learning algorithm ran. 
bool ran_learning;

unordered_map<unsigned int, frequency_struct> frequencies;

#endif // LEARNING_H_