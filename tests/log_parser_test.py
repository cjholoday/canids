import context #  Necessary so python3 finds modules
from defense.detection.mlids import fileio
import os

data_file = os.getcwd() + '/../data/logs/recording1.log'

can_msgs = fileio.log_parser(data_file)
for msg in can_msgs:
    print(msg)
