import can
can.rc['interface'] = 'socketcan'
can.rc['channel'] = 'vcan0'
from can.interfaces.interface import Bus

bus = Bus()
for msg in bus:
    print(msg.data)
