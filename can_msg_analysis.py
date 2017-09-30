import os

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.backends.backend_pdf import PdfPages

from defense import fileio

tick_size = 4


def plot_bit_data(plot_title, can_data):
    ones_count = np.zeros(64)
    for msg in can_data:
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
    plt.title(plot_title)
    plt.xlabel('ith Bit Position')
    plt.ylabel('Number of ones')

    plt.close()
    return fig

data_file1 = os.getcwd() + '/data/logs/recording1.log'
data_file2 = os.getcwd() + '/data/logs/recording2.log'

figures = []

can_msgs = fileio.log_parser(data_file1)
figures.append(plot_bit_data('Number of 1\'s in bit position i,\nRecording 1', can_msgs))

can_msgs = fileio.log_parser(data_file2)
figures.append(plot_bit_data('Number of 1\'s in bit position i,\nRecording 2', can_msgs))


pp = PdfPages('plots/bit_frequency.pdf')
for fig in figures:
    pp.savefig(fig)
pp.close()
