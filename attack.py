# attack.py
#
# purpose: provides an interface to all attack types
# note: attack.py must be placed in the top level directory so that the attack
#       submodule has access to other submodules' functions

from attack import attacks
import attack.manager

# TODO: implement cli
if __name__ == '__main__':
    attack_manager = attack.manager.AttackManager()
    attack_manager.max_time = 4

    attack_manager.reset()
    attacks.dos(attack_manager)

    attack_manager.reset()
    attacks.fuzz(attack_manager)

    attack_manager.reset()
    attacks.replay(attack_manager)
