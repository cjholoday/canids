# attack.py
#
# purpose: provides an interface to all attack types
# note: attack.py must be placed in the top level directory so that the attack
#       submodule has access to other submodules' functions

from attack import attacks
import attack.manager
import sys
import fractions
import random

import click

@click.command()
@click.option('--max-time', type=float, 
        help='End the attack after FLOAT seconds (default=unbounded)')
@click.option('--max-payloads', type=int, 
        help='End the attack after INTEGER payloads (default=unbounded)')
@click.option('--payload-delay', type=float, 
        help='Delay each payload by FLOAT seconds. Combines with --delay-prob (default=0.0)')
@click.option('--attack-prob', type=float, 
        help='Send every generated payload with probability FLOAT (default=1.0)')
@click.option('--delay-prob', type=float, 
        help='Delay each payload with probability FLOAT')
@click.option('-s', '--seed', type=int,
        help='Use seed INTEGER with the random number generator')
@click.option('-q', '--quiet', is_flag=True, 
        help='Suppress all output')
@click.argument('attack_type', type=click.Choice(list(attacks.attack_dict.keys())))
def run_attack(max_time, max_payloads, attack_prob, delay_prob, 
        payload_delay, quiet, attack_type, seed):
    """CAN Attack Suite. Currently supported attack types:

        'dos'

        \tDenial of service. Sends messages with all data as 0

        'fuzz'

        \tSends randomly generated messages with uniform probability

        'replay'

        \tCollects messages played on the bus and replays them with
        \tuniform probability. Eight messages are collected before the
        \tattack begins. After each payload is sent, another message
        \tis added to the replay buffer
    """

    manager = attack.manager.AttackManager()

    # Configure the attack manager with command options
    manager.max_time = max_time
    manager.max_payloads = max_payloads
    manager.payload_delay = payload_delay
    manager.quiet = quiet
    if attack_prob:
        manager.attack_prob = fractions.Fraction(attack_prob).limit_denominator()
    if delay_prob:
        manager.delay_prob = fractions.Fraction(delay_prob).limit_denominator()
    if seed: random.seed(seed)

    # run the attack
    attacks.attack_dict[attack_type](manager)

if __name__ == '__main__':
    run_attack()
