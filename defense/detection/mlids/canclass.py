class CANMessage:
    """Class for representing a CAN message"""
    def __init__(self, ts, msg_id, msg_data):
        self.timestamp = ts
        self.id = msg_id
        self.data = msg_data
