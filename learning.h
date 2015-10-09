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

//Stores the expected frequencies in frequencies
void storeMessages();

unordered_map<unsigned int, frequency_struct> frequencies;

#endif // LEARNING_H_