# attack.py
#
# purpose: provides an interface to all attack types
# note: attack.py must be placed in the top level directory so that the attack
#       submodule has access to other submodules' functions

from attack import attacks

if __name__ == '__main__':
    attacks.dos()
    attacks.fuzz()
    attacks.fuzz_replay()
