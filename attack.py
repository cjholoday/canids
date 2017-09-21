# attack.py
#
# purpose: provides an interface to all attack types
# note: attack.py must be placed in the top level directory so that the attack
#       submodule has access to other submodules' functions

from attack import attacks
from attack import manager
import click
import sys



@click.command()
@click.option('--max-time', type=float, help='todo')
@click.option('--max-payloads', type=int, help='todo')
@click.option('--attack-prob', nargs=2, type=int, help='todo')
@click.option('--delay-prob', nargs=2, type=int, default=None, help='todo')
@click.option('--payload-delay', type=int, help='todo')
@click.option('--hide-payloads', is_flag=True, help='todo')
@click.argument('attack_type', type=click.Choice(list(attacks.attack_dict.keys())))
def run_attack(max_time, max_payloads, attack_prob, delay_prob, 
        payload_delay, hide_payloads, attack_type):
    attack_manager = manager.AttackManager()

    # Configure the attack manager with command options
    if max_time: attack_manager.max_time = max_time
    if max_payloads: attack_manager.max_payloads = max_payloads
    if payload_delay: attack_manager.payload_delay = payload_delay
    attack_manager.show_sent_payloads = not hide_payloads

    # run the attack
    attacks.attack_dict[attack_type](attack_manager)

if __name__ == '__main__':
    run_attack()
