import os
import numpy as np
import json
import math
import tensorflow as tf

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
rec_1 = wd + '/data/logs/recording1.log'
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


def train_model():
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

        if i < len(can_msgs) - 1 and np.random.randint(0, 5) == 0:  # 20% chance of insertion
            rand_id = np.random.randint(0, 5001)
            new_time_stamp = (can_msgs[i].timestamp + can_msgs[i + 1].timestamp) / 2

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


def load_and_run_model():
    known_messages = {}
    with open(probability_file, 'r') as infile:
        temp_msgs = json.load(infile)

    for key in temp_msgs:
        known_messages[int(key, 16)] = temp_msgs[key]

    can_msgs = fileio.log_parser(rec_1)

    features = []
    labels = []
    seen_messages = {'Total': 0}
    previous_entropy = 0
    current_entropy = 0

    malicious_accuracy = [0, 0]
    classification_accuracy = [0, 0]

    with tf.Session() as sess:
        new_saver = tf.train.import_meta_graph('simple_nn_model.meta')
        new_saver.restore(sess, tf.train.latest_checkpoint('./'))

        x = tf.placeholder(tf.float32, [None, 4])
        graph = tf.get_default_graph()
        weights = graph.get_tensor_by_name('W:0')
        biases = graph.get_tensor_by_name('b:0')

        prediction = tf.nn.softmax(tf.matmul(x, weights) + biases)

        print('Restored model!')

        for i in range(0, len(can_msgs)):
            if (i - 1) % 1000 == 0:
                print('Processed ' + str(i - 1) + ' of ' + str(len(can_msgs)))

            seen_messages['Total'] = seen_messages['Total'] + 1
            if can_msgs[i].id_float not in seen_messages:
                seen_messages[can_msgs[i].id_float] = 0
            seen_messages[can_msgs[i].id_float] = seen_messages[can_msgs[i].id_float] + 1

            [p, q] = get_probability_distributions(can_msgs[i].id_float, known_messages,
                                                   seen_messages)

            current_entropy = calculate_entropy(seen_messages)
            if i == 1:
                previous_entropy = current_entropy

            features.append([can_msgs[i].id_float,
                             find_num_occurrences_in_last_second(i, can_msgs[i].id_float,
                                                                 can_msgs[i].timestamp, can_msgs),
                             calculate_relative_entropy(q, p), current_entropy - previous_entropy])

            labels.append([1, 0])

            if i < len(can_msgs) - 1 and np.random.randint(0, 10) == 0:  # 10% chance of insertion
                rand_id = np.random.randint(0, 5001)
                new_time_stamp = (can_msgs[i].timestamp + can_msgs[i + 1].timestamp) / 2

                seen_messages['Total'] = seen_messages['Total'] + 1
                if rand_id not in seen_messages:
                    seen_messages[rand_id] = 0
                seen_messages[rand_id] = seen_messages[rand_id] + 1

                [p, q] = get_probability_distributions(rand_id, known_messages,
                                                       seen_messages)

                features.append([rand_id,
                                 find_num_occurrences_in_last_second(i, rand_id,
                                                                     new_time_stamp, can_msgs),
                                 calculate_relative_entropy(q, p),
                                 current_entropy - previous_entropy])
                labels.append([0, 1])

                classification = sess.run(tf.argmax(prediction, 1), feed_dict={x: features})
                print('Injected ' + str(rand_id))
                is_caught = '\tWas Caught' if classification[len(classification) - 1] == 1 \
                    else '\tWas Not Caught'
                print(is_caught)
                if input() == 'q':
                    break
            else:
                print('No injection')

            previous_entropy = current_entropy

        classification = sess.run(tf.argmax(prediction, 1), feed_dict={x: features})

        for i in range(0, len(labels)):
            classification_accuracy[1] = classification_accuracy[1] + 1
            if labels[i][0] == 1 and classification[i] == 0:
                classification_accuracy[0] = classification_accuracy[0] + 1
            elif labels[i][1] == 1:
                malicious_accuracy[1] = malicious_accuracy[1] + 1
                if classification[i] == 1:
                    malicious_accuracy[0] = malicious_accuracy[0] + 1
                    classification_accuracy[0] = classification_accuracy[0] + 1

        print('Classification accuracy = ' + str(classification_accuracy[0] / classification_accuracy[1] * 100) + '%')
        print('Percentage caught = ' + str(malicious_accuracy[0] / malicious_accuracy[1] * 100) + '%')


if __name__ == "__main__":
    # train_model()
    load_and_run_model()
