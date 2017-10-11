import matplotlib.pyplot as plt
import numpy as np

tick_size = 4
title_size = 10
label_size = 6


def plot_bit_occurrences(plot_title, can_data):
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
    plt.title(plot_title, fontsize=title_size)
    plt.xlabel('ith Bit Position', fontsize=label_size)
    plt.ylabel('Number of ones', fontsize=label_size)

    plt.close()

    i_string = 'Bit Position'
    count_string = 'Number of ones'
    for i in range(0, 64):
        i_string = i_string + ',' + str(i)
        count_string = count_string + ',' + str(ones_count[i])

    return [fig, i_string + '\n' + count_string]


def plot_id_occurrences(plot_title, occurrence_data):
    ids = np.array([])
    freqs = np.array([])
    for id in occurrence_data:
        ids = np.append(ids, [id])
        freqs = np.append(freqs, [occurrence_data[id]])

    fig, ax = plt.subplots()

    y_pos = np.arange(len(ids))

    plt.bar(y_pos, freqs)
    plt.xticks(y_pos, ids, fontsize=tick_size)
    plt.title(plot_title, fontsize=title_size)
    plt.xlabel('CAN Message ID', fontsize=label_size)
    plt.ylabel('Number of occurrences', fontsize=label_size)

    fig.autofmt_xdate()

    plt.close()
    return fig


def plot_id_frequency_distribution(plot_title, freq_data):
    freqs = np.array([])
    occurrences = np.array([])
    freq_times_occurrences = 0
    for freq in sorted(freq_data):
        freqs = np.append(freqs, [freq])
        occurrences = np.append(occurrences, [freq_data[freq]])
        freq_times_occurrences = freq_times_occurrences + freq*freq_data[freq]

    fig, ax = plt.subplots()
    y_pos = np.arange(len(freqs))

    plt.bar(y_pos, occurrences)
    plt.xticks(y_pos, freqs, fontsize=tick_size)
    plt.title(plot_title, fontsize=title_size)
    plt.xlabel('Frequency', fontsize=label_size)
    plt.ylabel('Number of Occurrences of Frequency', fontsize=label_size)

    fig.autofmt_xdate()

    plt.close()

    return [fig, freq_times_occurrences / np.sum(occurrences)]


def plot_id_avg_frequency(plot_title, freq_data):
    ids = []
    freqs = []

    for id in freq_data:
        ids.append(id)
        freqs.append(freq_data[id]['Average Frequency'])

    fig, ax = plt.subplots()
    y_pos = np.arange(len(ids))

    plt.bar(y_pos, freqs)
    plt.xticks(y_pos, ids, fontsize=tick_size)
    plt.title(plot_title, fontsize=title_size)
    plt.xlabel('CAN ID', fontsize=label_size)
    plt.ylabel('Frequency', fontsize=label_size)

    fig.autofmt_xdate()

    plt.close()

    return fig


def plot_entropies(plot_title, timestamps, entropies):
    fig, ax = plt.subplots()

    plt.plot(timestamps, entropies)
    plt.title(plot_title, fontsize=title_size)
    plt.xlabel('Timestamp', fontsize=label_size)
    plt.ylabel('Entropy', fontsize=label_size)

    fig.autofmt_xdate()

    plt.close()

    return fig
