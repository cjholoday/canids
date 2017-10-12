import os
import numpy as np

from defense import fileio
import defense.detection.mlids.simple_nn.simple_nn_impl as classifier

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
    can_msgs = fileio.log_parser(rec_2)

    features = []
    labels = []

    for i in range(0, len(can_msgs)):
        if (i - 1) % 1000 == 0:
            print('Processed ' + str(i) + ' of ' + str(len(can_msgs)))
        features.append([can_msgs[i].id_float,
                         find_num_occurrences_in_last_second(i, can_msgs[i].id_float,
                                                             can_msgs[i].timestamp, can_msgs)])
        labels.append([1, 0])

        if i < len(can_msgs) - 1 and np.random.randint(0, 5) == 0:  # 20% chance of insertion
            rand_id = np.random.randint(0, 5001)
            new_time_stamp = (can_msgs[i].timestamp + can_msgs[i+1].timestamp) / 2
            features.append([rand_id,
                             find_num_occurrences_in_last_second(i, rand_id,
                                                                 new_time_stamp, can_msgs)])
            labels.append([0, 1])

    print('Processed all data!')
    nn = classifier.SimpleNN(features, labels)
