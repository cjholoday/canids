class CANMessage:
    """Class for representing a CAN message"""
    def __init__(self, ts, msg_id, msg_data):
        self.timestamp = ts
        self.id = msg_id
        self.data = msg_data

    def __repr__(self):
        return 'ID: 0x' + str(hex(int(self.id, 2)))[2:].zfill(3) + ' | DATA: 0x' \
               + str(hex(int(self.data, 2)))[2:].zfill(16)
