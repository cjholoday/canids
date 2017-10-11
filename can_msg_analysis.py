import os
from matplotlib.backends.backend_pdf import PdfPages
import plotter.plotters as plotter
import csv
import json

from defense import fileio

wd = os.getcwd()
files = [wd + '/data/logs/recording1.log', wd + '/data/logs/recording2.log']


def bit_occurrence_analyzer(file_data_map):
    figures = []
    all_msg_data = []

    with open('data/analysis_dump/bit_occurrences.csv', 'w') as out_file:
        for file in file_data_map:
            [temp_fig, out_string] = plotter.plot_bit_occurrences('Number of 1\'s in bit position '
                                                                  'i,'
                                                                  '\n' + file, file_data_map[file])
            all_msg_data = all_msg_data + file_data_map[file]
            figures.append(temp_fig)
            out_file.write(out_string + '\n')

        [temp_fig, out_string] = plotter.plot_bit_occurrences('Number of 1\'s in bit position i,'
                                                              '\n' + 'All', all_msg_data)
        figures.append(temp_fig)
        out_file.write(out_string)

    pp = PdfPages('plots/bit_occurrences.pdf')
    for fig in figures:
        pp.savefig(fig)
    pp.close()


def id_occurrence_analyzer(file_data_map):
    id_occurrence_figures = []
    all_id_freq_map = {}
    for file in file_data_map:
        id_freq_map = {}
        for can_msg in file_data_map[file]:
            if can_msg.id not in id_freq_map:
                id_freq_map[can_msg.id] = 1
            else:
                id_freq_map[can_msg.id] = id_freq_map[can_msg.id] + 1
            if can_msg.id not in all_id_freq_map:
                all_id_freq_map[can_msg.id] = 1
            else:
                all_id_freq_map[can_msg.id] = all_id_freq_map[can_msg.id] + 1

        id_occurrence_figures.append(plotter.plot_id_occurrences('CAN ID Occurrences,'
                                                                 '\n' + file, id_freq_map))
    id_occurrence_figures.append(plotter.plot_id_occurrences('CAN ID Occurrences,'
                                                             '\nAll', all_id_freq_map))

    with open('data/analysis_dump/id_occurrences.json', 'w') as json_file:
        json_file.write(json.dumps(all_id_freq_map, sort_keys=True, indent=4))

    pp = PdfPages('plots/id_occurrences.pdf')
    for fig in id_occurrence_figures:
        pp.savefig(fig)
    pp.close()


def id_frequency_analyzer(file_data_map):
    all_data = {}
    for file in file_data_map:
        i = 0
        while i < len(file_data_map[file]) - 1:
            one_second_data = {}
            t_i = file_data_map[file][i].timestamp
            one_second_data[file_data_map[file][i].id] = 1
            i = i + 1
            if i >= len(file_data_map[file]):
                break
            t_t = file_data_map[file][i].timestamp
            while t_t <= t_i + 1:
                if file_data_map[file][i].id not in one_second_data:
                    one_second_data[file_data_map[file][i].id] = 1
                else:
                    one_second_data[file_data_map[file][i].id] \
                        = one_second_data[file_data_map[file][i].id] + 1
                i = i + 1
                if i >= len(file_data_map[file]):
                    break
                t_t = file_data_map[file][i].timestamp

            for msg_id in one_second_data:
                if msg_id not in all_data:
                    all_data[msg_id] = {}
                    all_data[msg_id][one_second_data[msg_id]] = 1
                else:
                    if one_second_data[msg_id] not in all_data[msg_id]:
                        all_data[msg_id][one_second_data[msg_id]] = 1
                    else:
                        all_data[msg_id][one_second_data[msg_id]] \
                            = all_data[msg_id][one_second_data[msg_id]] + 1

    figures = []
    for identifier in all_data:
        [fig, avg_freq] = plotter.plot_id_frequency_distribution('Frequency Distribution of ID 0x'
                                                                 + identifier,
                                                                 all_data[identifier])
        figures.append(fig)
        all_data[identifier]['Average Frequency'] = avg_freq

    pp = PdfPages('plots/id_frequency_distribution.pdf')
    for fig in figures:
        pp.savefig(fig)
    pp.close()

    with open('data/analysis_dump/id_frequency_distribution.json', 'w') as json_file:
        json_file.write(json.dumps(all_data, indent=4))

    pp = PdfPages('plots/id_average_frequency.pdf')
    fig = plotter.plot_id_avg_frequency('Average Frequency of Each CAN ID', all_data)
    pp.savefig(fig)
    pp.close()


if __name__ == "__main__":
    can_msgs = {}
    for file in files:
        can_msgs[file[file.find('recording'):]] = fileio.log_parser(file)

    bit_occurrence_analyzer(can_msgs)
    id_occurrence_analyzer(can_msgs)
    id_frequency_analyzer(can_msgs)
