import os
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np
import matplotlib.pyplot as plt
from defense.detection.mlids import fileio

data_file = os.getcwd() + '/data/logs/recording1.log'
tick_size = 4

ones_count = np.zeros(64)

can_msgs = fileio.log_parser(data_file)

for msg in can_msgs:
    if len(msg.data) != 64:
        raise ValueError('Unexpected CAN message data length: ' + str(len(msg.data)))
    for i in range(0, 64):
        if msg.data[i] == '1':
            ones_count[i] = ones_count[i] + 1

fig, ax = plt.subplots()

bit_pos_nums = np.linspace(0, 63, num=64, dtype=int)
y_pos = np.arange(len(bit_pos_nums))

plt.bar(y_pos, ones_count)
plt.xticks(y_pos, bit_pos_nums, fontsize=tick_size, rotation='vertical')

plt.close()

pp = PdfPages('plots/bit_frequency.pdf')
pp.savefig(fig)
pp.close()
