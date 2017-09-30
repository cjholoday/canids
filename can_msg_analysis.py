import os
from matplotlib.backends.backend_pdf import PdfPages
import plotter.plotters as plotter
import csv

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

    with open('data/analysis_dump/id_occurrences.csv', 'w') as csv_file:
        writer = csv.writer(csv_file)
        for key, value in all_id_freq_map.items():
            writer.writerow([key, value])

    pp = PdfPages('plots/id_occurrences.pdf')
    for fig in id_occurrence_figures:
        pp.savefig(fig)
    pp.close()


def id_frequency_analyzer(file_data_map):
    return


def msg_data_entropy_analyzer(file_data_map):
    return


def id_based_entropy_analyzer(file_data_map):
    return


if __name__ == "__main__":
    can_msgs = {}
    for file in files:
        can_msgs[file[file.find('recording'):]] = fileio.log_parser(file)

    bit_occurrence_analyzer(can_msgs)
    id_occurrence_analyzer(can_msgs)
