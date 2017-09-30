import os
import context
from defense import fileio

data_file = os.getcwd() + '/../data/logs/recording1.log'

can_msgs = fileio.log_parser(data_file)
for msg in can_msgs:
    print(msg)
