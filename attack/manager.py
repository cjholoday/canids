# manager.py
#
# purpose: handle management of command line configuration of attacks

import timeit
import time
import fractions
import random


import can


def print_msg(can_msg):
    data_str = ''
    for byte in can_msg.data:
        data_str += '{:02x} '.format(byte)

    print('{}  {:03x}   [{}]  {}'.format( can.rc['channel'], 
        can_msg.arbitration_id, len(can_msg.data), data_str))


def fraction_roll(success_rate):
    """Returns true with a given success rate"""
    return random.randint(1, success_rate.denominator) <= success_rate.numerator


class AttackManager:
    def __init__(self):
        """Set default configuration"""

        self.max_time = None
        self.max_payloads = None

        # we start counting time from the first call to self.should_attack()
        self.start_time = None
        self.payloads_sent = 0

        self.attack_prob = None
        self.delay_prob = None

        self.payload_delay = None
        
        self.quiet = False

    def reset(self):
        """Sets this manager up as if it hasn't been used in an attack"""
        self.start_time = None
        self.payloads_sent = 0

    def should_continue(self):
        """Returns true if the attack should continue"""

        if self.max_payloads and self.payloads_sent >= self.max_payloads:
            return False
        elif self.max_time and self.start_time:
            return timeit.default_timer() - self.start_time <= self.max_time
        else:
            return True

    def attack(self, payload):
        """Manages an attack using configuration from a CLI"""
        if not self.attack_prob or fraction_roll(self.attack_prob):
            if self.payload_delay and (
                    not self.delay_prob or fraction_roll(self.delay_prob)):
                if not self.quiet: print("(payload delay)")
                time.sleep(self.payload_delay)

            # start the timer once the first payload is sent
            if self.max_time and self.start_time == None:
                if not self.quiet: print("(start timer)")
                self.start_time = timeit.default_timer()

            bus.send(payload)
            self.payloads_sent += 1
            if not self.quiet: print_msg(payload)
        else:
            if not self.quiet: print("(skip payload)")
