import sys


import click
import can
from defense.detection.ruleids import invariants


def detect_attacks(detection_q, quiet, channel):
    """Runs the invariant based IDS

    detection_q is used to communicate CAN payloads to a calling process
    """
    if channel == None:
        channel = 'vcan0'
    can.rc['channel'] = channel
    can.rc['interface'] = 'socketcan_ctypes'
    bus = can.interface.Bus()

    invariants.init()

    msg_log = []
    idx_faucet = 0
    while True:
        can_msg = bus.recv(timeout=1)

        can_msg.index = idx_faucet
        can_msg.defense = 'ruleids'
        idx_faucet += 1

        if can_msg:
            if not quiet:
                print(can_msg) # useful for debugging
            msg_log.append(can_msg)
            invariant_broken = invariants.check_invariants(msg_log)
            if invariant_broken:
                detection_q.put(msg_log[-1])
                attack_caught(msg_log, invariant_broken, quiet)


def attack_caught(msg_log, invariant_broken, quiet):
    """Print out info on the attack that was detected"""
    if quiet:
        return

    print("ATTACK DETECTED")
    print("    Invariant type: {}".format(invariant_broken))
    print("    Messages seen so far: {}".format(len(msg_log)))
    print("    (The violating message is above)")
    print("")



@click.command()
@click.option('-c', '--channel', type=str,
        help='Specify which channel on which to listen for attacks (e.g. vcan0)'
             ' (default=vcan0)')
@click.option('-q', '--quiet', is_flag=True,
        help='Suppress all output')
def cli_wrapper(quiet, channel):
    detect_attacks(DummyQueue(), quiet, channel)


class DummyQueue:
    """
    We don't need a detection queue unless 'detect_attacks' is being called
    from another process. Use a dummy queue when we are only using one process
    """
    def put(self, can_msg):
        pass # do nothing


if __name__ == '__main__':
    cli_wrapper()
