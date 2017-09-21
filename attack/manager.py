import timeit
import time

import can

can.rc['interface'] = 'socketcan_ctypes'
can.rc['channel'] = 'vcan0'
bus = can.interface.Bus()

def print_msg(can_msg):
    # TODO: fix display of data portion
    # TODO: move this to a module where other teams can use it
    print('{}  {:03x}   [{}]  {}'.format( can.rc['channel'], 
        can_msg.arbitration_id, len(can_msg.data), 'todo'))

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

    def reset(self):
        """Sets this manager up as if it hasn't been used in an attack"""
        self.start_time = None
        self.payloads_sent = 0

    def should_continue(self):
        """Returns true if the attack should continue"""

        if self.max_payloads and self.max_payloads >= self.payloads_sent:
            return False
        elif self.max_time and self.start_time:
            return timeit.default_timer() - self.start_time <= self.max_time
        else:
            return True

    def should_attack(self):
        """Returns true if a payload should be sent"""
        if self.max_time and self.start_time == None:
            self.start_time = timeit.default_timer()

        # TODO: implement attack probability
        self.payloads_sent += 1
        return True
        

    def delay_payload(self):
        """Delays the payload from being sent if a delay was configured"""

        # TODO: implement delay probability
        if self.payload_delay:
            time.sleep(self.payload_delay)

    def display_payload(self, can_msg):
        """Display the payload if the user configured display of messages"""
        # TODO: add option to display / not display
        print_msg(can_msg)

    def attack(self, payload):
        """Manages an attack using configuration from a CLI"""
        if self.should_attack():
            self.delay_payload()
            self.display_payload(payload)
            bus.send(payload)
