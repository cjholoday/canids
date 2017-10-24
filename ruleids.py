import sys

import can
from defense.detection.ruleids import invariants


def attack_caught(msg_log, invariant_broken):
    # XXX TODO: print out more info
    print("Attack detected")

def detect_attacks():
    invariants.init()

    msg_log = []
    while True:
        can_msg = bus.recv(timeout=1)
        if can_msg:
            print(can_msg) # useful for debugging
            msg_log.append(can_msg)
            invariant_broken = invariants.check_invariants(msg_log)
            if invariant_broken:
                attack_caught(msg_log, invariant_broken)

if __name__ == '__main__':
    # setup the CAN bus API
    can.rc['interface'] = 'socketcan_ctypes'
    can.rc['channel'] = 'vcan0'
    bus = can.interface.Bus()

    detect_attacks()
