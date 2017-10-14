import os
import numpy as np
import json
import math

from defense import fileio
import defense.detection.mlids.simple_nn.simple_nn_impl as classifier


def calculate_entropy(map_of_ids):
    system_entropy = 0
    for msg_id in map_of_ids:
        probability = map_of_ids[msg_id] / map_of_ids['Total']
        system_entropy = system_entropy + probability * math.log(1.0 / probability)

    return system_entropy


def calculate_relative_entropy(normal, measured):
    if normal == 0:
        return 100
    return measured * math.log(measured / normal)


def get_probability_distributions(msg_id, baseline, empirical):
    if msg_id in baseline:
        q = baseline[msg_id]['Probability']
    else:
        q = 0
    p = empirical[msg_id] / empirical['Total']

    return [p, q]

probability_file = 'data/analysis_dump/id_occurrences.json'

wd = os.getcwd()
rec_2 = wd + '/data/logs/recording2.log'
#  files = [wd + '/data/logs/recording1.log', wd + '/data/logs/recording2.log']


def find_num_occurrences_in_last_second(index, msg_id, timestamp, messages):
    count = 0
    for j in range(index, 0, -1):
        if timestamp - messages[j].timestamp <= 1:
            if messages[j].id_float == msg_id:
                count = count + 1
        else:
            break
    return count


if __name__ == "__main__":
    known_messages = {}
    with open(probability_file, 'r') as infile:
        temp_msgs = json.load(infile)

    for key in temp_msgs:
        known_messages[int(key, 16)] = temp_msgs[key]

    can_msgs = fileio.log_parser(rec_2)

    features = []
    labels = []
    seen_messages = {'Total': 0}
    previous_entropy = 0
    current_entropy = 0

    for i in range(0, len(can_msgs)):
        if (i - 1) % 10000 == 0:
            print('Processed ' + str(i - 1) + ' of ' + str(len(can_msgs)))

        seen_messages['Total'] = seen_messages['Total'] + 1
        if can_msgs[i].id_float not in seen_messages:
            seen_messages[can_msgs[i].id_float] = 0
        seen_messages[can_msgs[i].id_float] = seen_messages[can_msgs[i].id_float] + 1

        [p, q] = get_probability_distributions(can_msgs[i].id_float, known_messages, seen_messages)

        current_entropy = calculate_entropy(seen_messages)
        if i == 1:
            previous_entropy = current_entropy

        features.append([can_msgs[i].id_float,
                         find_num_occurrences_in_last_second(i, can_msgs[i].id_float,
                                                             can_msgs[i].timestamp, can_msgs),
                         calculate_relative_entropy(q, p), current_entropy - previous_entropy])
        labels.append([1, 0])

        if i < len(can_msgs) - 1 and np.random.randint(0, 10) == 0:  # 20% chance of insertion
            rand_id = np.random.randint(0, 5001)
            new_time_stamp = (can_msgs[i].timestamp + can_msgs[i+1].timestamp) / 2

            seen_messages['Total'] = seen_messages['Total'] + 1
            if rand_id not in seen_messages:
                seen_messages[rand_id] = 0
            seen_messages[rand_id] = seen_messages[rand_id] + 1

            [p, q] = get_probability_distributions(rand_id, known_messages,
                                                   seen_messages)

            features.append([rand_id,
                             find_num_occurrences_in_last_second(i, rand_id,
                                                                 new_time_stamp, can_msgs),
                             calculate_relative_entropy(q, p), current_entropy - previous_entropy])
            labels.append([0, 1])

        previous_entropy = current_entropy

    print('Processed all data!')
    nn = classifier.SimpleNN(features, labels)
