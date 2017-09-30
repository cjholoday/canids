class CANMessage:
    """Class for representing a CAN message"""
    def __init__(self, ts, msg_id, msg_data):
        self.timestamp = float(ts)
        self.id = str(msg_id)
        self.data = str(msg_data)

    def __repr__(self):
        return 'ID: 0x' + self.id + ' | DATA: 0x' \
               + str(hex(int(self.data, 2)))[2:].zfill(16)
