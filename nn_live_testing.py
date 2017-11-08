import can
import click
import os

from defense.detection.mlids.classifier_impl import MessageClassifier
from defense.canclass import CANMessage


def detect_attacks(detection_q, quiet, channel):
    """Runs the neural network based IDS

    detection_q is used to communicate CAN payloads to a calling process
    """
    if channel is None:
        channel = 'vcan0'
    can.rc['channel'] = channel
    can.rc['interface'] = 'socketcan_ctypes'
    bus = can.interface.Bus()

    ml_ids = MessageClassifier(os.getcwd() + '/ml_ids_model')

    seen_messages = {'Total': 0}
    msg_log = []
    while True:
        can_msg = bus.recv(timeout=1)
        if can_msg:
            new_msg = CANMessage(can_msg.timestamp,
                                 "{0:#0{1}X}".format(can_msg.arbitration_id, 5)[2:],
                                 0)  # Data section is not useful and may screw things up
            msg_log.append(new_msg)
            if not quiet:
                print(can_msg)
                # print(msg_log[len(msg_log) - 1])

            seen_messages['Total'] = seen_messages['Total'] + 1
            if new_msg.id_float not in seen_messages:
                seen_messages[new_msg.id_float] = 0
            seen_messages[new_msg.id_float] = seen_messages[new_msg.id_float] + 1

            prediction = ml_ids.prediction_wrapper(new_msg, msg_log, seen_messages)

            if prediction is True:
                print(new_msg.id + ' at timestamp ' + str(new_msg.timestamp) + ' is MALICIOUS')

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
