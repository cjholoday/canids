from logger.can_message_logger import CANLogger
from defense.canclass import CANMessage

can_logger = CANLogger()

while True:
    next_message = can_logger.get_next_message()
    if next_message is None:
        print('No message yet!')
        continue
    print('ID = ' + str(next_message.arbitration_id))
    print('Message = ' + str(next_message.data))
