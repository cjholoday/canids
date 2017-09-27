'''
This file contains functions that are intented to return useful
information from a CAN log file
Author: Lee Moore - 8/3/17
'''

import time

HEXDIV = 2

class Message:
    time = None
    can_id = None
    data = []
    def print_Message(self):
        print('time:', self.time, 'id:'.expandtabs(6), self.can_id, 'data:', self.data)
    def __init__(self):
        self.time = None
        self.can_id = None
        self.data = []

def split_message(log_message, can_Message):
    #set message time
    can_Message.time = float(log_message[1:log_message.find(')')])
    #find (last) location of the pound sign(in message)
    pound = log_message.rfind('#')
    #find id then convert to int
    can_Message.can_id =  int(log_message[pound-3:pound], 16)
    #find message then separate into individual bytes
    total_mess = log_message[pound+1:log_message.rfind('\n')]
    #can_Message.data = []
    ###NEEDS UPDATING ON HOW TO DIVIDE###
    for i in range(int(len(total_mess)/HEXDIV)):
        can_Message.data.append(int(total_mess[i*HEXDIV:i*HEXDIV+HEXDIV],16))
    #can_Message.print_Message()

def return_message_list(infile):
    messages = []
    can_log = open(infile, 'r')

    #extract information form each log file entry
    for message in can_log:
        temp = Message()
        split_message(message,temp)
        messages.append(temp)

    #set first message to timestamp 0
    for each in messages[1:]:
        each.time -= messages[0].time
    messages[0].time = 0

    can_log.close()
    return messages

if __name__ == '__main__':
    messages = return_message_list('recording.log')

    print('message output')
    time.sleep(2)

    for temp in messages[0:100]:
        temp.print_Message()
