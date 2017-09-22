# attack.py
#
# purpose: provides an interface to all attack types
# note: attack.py must be placed in the top level directory so that the attack
#       submodule has access to other submodules' functions

from attack import attacks
import attack.manager
import click
import sys
import fractions


@click.command()
@click.option('--max-time', type=float, help='todo')
@click.option('--max-payloads', type=int, help='todo')
@click.option('--attack-prob', type=float, help='todo')
@click.option('--delay-prob', type=float, help='todo')
@click.option('--payload-delay', type=float, help='todo')
@click.option('--hide-payloads', is_flag=True, help='todo')
@click.argument('attack_type', type=click.Choice(list(attacks.attack_dict.keys())))
def run_attack(max_time, max_payloads, attack_prob, delay_prob, 
        payload_delay, hide_payloads, attack_type):
    manager = attack.manager.AttackManager()

    # Configure the attack manager with command options
    manager.max_time = max_time
    manager.max_payloads = max_payloads
    manager.payload_delay = payload_delay
    manager.show_sent_payloads = not hide_payloads
    if attack_prob:
        manager.attack_prob = fractions.Fraction(attack_prob).limit_denominator()
    if delay_prob:
        manager.delay_prob = fractions.Fraction(delay_prob).limit_denominator()

    # run the attack
    attacks.attack_dict[attack_type](manager)

if __name__ == '__main__':
    run_attack()
