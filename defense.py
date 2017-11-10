import sys
import multiprocessing
from multiprocessing import Process


import click
import ruleids


def main():
    payloads = multiprocessing.Queue()
    channel = 'vcan0'
    
    # start defense
    ruleids_defense = Process(target=ruleids.detect_attacks, 
                              args=(payloads, True, channel,))
    ruleids_defense.start()

    # FIXME
    ml_defense = Process(target=dummy_machine_learning_defense, 
                         args=(payloads,))
    ml_defense.start()

    
    # start responding to malicious messages
    response_scheme = Process(target=dummy_response_scheme, #FIXME
                              args=(payloads,))
    response_scheme.start()

def dummy_response_scheme(payloads):
    while True:
        # Block until we get a can msg the defense found as malicious
        payload = payloads.get() 

        print("Response scheme received payload: ")
        print("\t{}".format(payload))
        print("\tmessage num: {}, caught by: {}".format(payload.index, payload.defense))
        print("")

def dummy_machine_learning_defense(payloads):
    pass # do nothing

if __name__ == '__main__':
    main()
