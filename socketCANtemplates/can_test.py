import can
# can.rc['interface'] = 'socketcan_native' # for python 3.3+
can.rc['interface'] = 'socketcan_native'
can.rc['channel'] = 'vcan0'
from can.interfaces.interface import Bus

bus = Bus()
for msg in bus:
    print(msg.data)
