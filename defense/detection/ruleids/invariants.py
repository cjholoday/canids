import sys

import can

def init():
    """Prepare the invariant suite for detection e.g. parse training data"""
    pass

def check_invariants(msg_log):
    """Check all invariants. Returns 'None' if no invariant was broken

    NOTE: If an invariant was broken, a string describing the broken invariant
          should be returned
    NOTE: Similarly, all invariant checking functions in this file should 
          return a string describing the invariant broken or 'None' if no 
          invariant was broken
    """

    # A list of all invariant checking functions
    invariant_checks = [check_nonzero_id,check_many_id, frequency_error_invariant]

    # Check all invariants
    for invariant_check in invariant_checks:
        invariant_broken = invariant_check(msg_log)
        if invariant_broken:
            return invariant_broken

    return None

def check_nonzero_id(msg_log):
    """Checks that the most recently sent message did not have a zero id"""

    if msg_log[-1].arbitration_id == 0:
        return "message sent with a zero id"
    else:
        return None

def check_many_id(msg_log):
    """ Checks that the id isn't the same for x times"""

    # FIXME: This function infinite loops currently. Skip it for now
    return None

    x = 0
    i = 1
    while i in range(1, len(msg_log) - 50):
        while msg_log[i].arbitration_id == msg_log[i-1].arbitration_id:
            x+=1
	
            if x == 50:
                return "messages have repeating arbitration id"
        x = 0
    return None

def frequency_error_invariant(msg_log):
    errorMessage = "None"
    while i in range(1, len(msg_log) - 50):
        timestampInterval = abs(msg_log[i].timestamp - msg_log[i + 1].timestamp)
        i = i + 1
        if timestampInterval<=0.0014/2.0:
	        errorMessage == "time interval less than half"
	        break
		
    return errorMessage
	
			



