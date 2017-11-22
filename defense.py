import sys
import signal
import os
import multiprocessing
from time import sleep
from multiprocessing import Process

from defense.detection.mlids.classifier_impl import MessageClassifier


import click
import ruleids


@click.command()
@click.option('-t', '--time', type=float, 
        help='Sets length of defense (default=unbounded)')
@click.option('--quiet-ruleids', is_flag=True, 
        help='Suppress ruleids output')
@click.option('--quiet-ml', is_flag=True, 
        help='Suppress machine learning output')
@click.option('--quiet-response', is_flag=True, 
        help='Suppress response output')
@click.option('-q', '--quiet', is_flag=True, 
        help='Suppress all output')
@click.option('-c', '--channel', type=str,
        help='Channel on which to listen for payloads (e.g. vcan0, can0)'
        ' (default=vcan0)')
def main(time, channel, quiet, quiet_ruleids, quiet_ml, quiet_response):
    payloads = multiprocessing.Queue()
    if channel is None:
        channel = 'vcan0'

    if quiet:
        quiet_ruleids = True
        quiet_response = True
        quiet_ml = True
    
    ruleids_defense = Process(target=ruleids.detect_attacks, 
                              args=(payloads, quiet_ruleids, channel,))

    # FIXME
    ml_ids = MessageClassifier(os.getcwd() + '/ml_ids_model')
    ml_defense = Process(target=ml_ids.detect_attacks,
                         args=(payloads, quiet_ml, channel))

    # FIXME
    response_scheme = Process(target=dummy_response_scheme, 
                              args=(payloads, quiet_response))

    ruleids_defense.start()
    ml_defense.start()
    response_scheme.start()

    if time is not None:
        sleep(time)

        # generate keyboard interrupts to quit out of all processes
        os.kill(ruleids_defense.pid, signal.SIGINT)
        os.kill(ml_defense.pid,      signal.SIGINT)
        os.kill(response_scheme.pid, signal.SIGINT)
        os.kill(os.getpid(),         signal.SIGINT)

def dummy_response_scheme(payloads, quiet):
    while True:
        # Block until we get a can msg the defense found as malicious
        payload = payloads.get() 
        if not quiet:
            print("Response scheme received payload: ")
            print("\t{}".format(payload))
            print("\tmessage num: {}, caught by: {}".format(payload.index, payload.defense))
            print("")

def dummy_machine_learning_defense(payloads, quiet, channel):
    pass # do nothing

if __name__ == '__main__':
    main()
