import os
import numpy as np

from defense import fileio
import defense.detection.mlids.simple_nn.simple_nn_impl as classifier

wd = os.getcwd()
rec_2 = wd + '/data/logs/recording2.log'
#  files = [wd + '/data/logs/recording1.log', wd + '/data/logs/recording2.log']


if __name__ == "__main__":
    can_msgs = fileio.log_parser(rec_2)

    features = []
    labels = []

    for msg in can_msgs:
        features.append([msg.id_float])
        labels.append([1, 0])

    for _ in range(10000):
        features.append([np.random.randint(0, 5001)])
        labels.append([0, 1])

    nn = classifier.SimpleNN(features, labels)
