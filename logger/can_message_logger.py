import can
from can.interfaces.interface import Bus


class CANLogger():
    def __init__(self):
        can.rc['interface'] = 'socketcan_native'
        can.rc['channel'] = 'vcan0'
        can.rc['bitrate'] = 500000
        self.bus = Bus()
        self.buffered_reader = can.BufferedReader()
        self.notifier = can.Notifier(self.bus, [self.buffered_reader])

    def get_next_message(self):
        return self.buffered_reader.get_message()
