import numpy as np
import tensorflow as tf


class MessageClassifierTrainer:
    def __init__(self, model_destination_dir=None):
        """
        Class for training DNN classifier in TensorFlow

        :param model_destination_dir: Directory to save model to
        """
        print('Setting up DNNClassifier')
        self.model_dir = model_destination_dir
        self.msg_id_column = tf.feature_column.numeric_column('msg_id', dtype=tf.float32)
        self.occurrences_column = tf.feature_column.numeric_column('occurrences_in_last_s',
                                                                   dtype=tf.float32)
        self.rel_entropy_column = tf.feature_column.numeric_column('relative_entropy',
                                                                   dtype=tf.float32)
        self.delta_entropy = tf.feature_column.numeric_column('delta_entropy', dtype=tf.float32)

        self.classifier \
            = tf.estimator.DNNClassifier(feature_columns=[self.msg_id_column,
                                                          self.occurrences_column,
                                                          self.rel_entropy_column,
                                                          self.delta_entropy],
                                         hidden_units=[4, 8, 16, 32, 16, 8, 4, 2],
                                         model_dir=self.model_dir,
                                         optimizer=tf.train.ProximalAdagradOptimizer(
                                             learning_rate=0.1,
                                             l1_regularization_strength=0.001
                                         ),
                                         n_classes=2,
                                         weight_column='weights')
        print('Finished setting up DNNClassifier')

    def train_classifier(self, msgs, labels):
        """
        Trains DNN Classifier

        :param msgs: list of features corresponding to each message in the form
        [[Message ID, Occurrences in last second, Relative Entropy, Change in System Entropy,
        weight]]
        :param labels: list of labels for each message, 1 for malicious and 0 for normal
        :return: None
        :raises ValueError: if the number of messages and the number of labels are not equal
        """
        if len(msgs) != len(labels):
            raise ValueError('len(msgs) not equal to len(labels): ' + str(len(msgs))
                             + ' != ' + str(len(labels)))

        msg_ids = []
        occurrences = []
        rel_entropies = []
        delta_entropies = []
        weights = []
        for msg in msgs:
            msg_ids.append(msg[0])
            occurrences.append(msg[1])
            rel_entropies.append(msg[2])
            delta_entropies.append(msg[3])
            weights.append(msg[4])

        x = {'msg_id': np.array(msg_ids),
             'occurrences_in_last_s': np.array(occurrences),
             'relative_entropy': np.array(rel_entropies),
             'delta_entropy': np.array(delta_entropies),
             'weights': np.array(weights)}

        y = np.array(labels)

        train_input_fn = tf.estimator.inputs.numpy_input_fn(x, y, num_epochs=None, shuffle=True)
        self.classifier.train(train_input_fn, steps=1000)

        print('Trained classifier!')

    def test_classifier(self, msgs, labels):
        """
        Tests DNN Classifier

        :param msgs: list of features corresponding to each message in the form
        [[Message ID, Occurrences in last second, Relative Entropy, Change in System Entropy]]
        :param labels: list of labels for each message, 1 for malicious and 0 for normal
        :return: None
        :raises ValueError: if the number of messages and the number of labels are not equal
        """
        if len(msgs) != len(labels):
            raise ValueError('len(msgs) not equal to len(labels): ' + str(len(msgs))
                             + ' != ' + str(len(labels)))

        msg_ids = []
        occurrences = []
        rel_entropies = []
        delta_entropies = []
        for msg in msgs:
            msg_ids.append(msg[0])
            occurrences.append(msg[1])
            rel_entropies.append(msg[2])
            delta_entropies.append(msg[3])

        x = {'msg_id': np.array(msg_ids),
             'occurrences_in_last_s': np.array(occurrences),
             'relative_entropy': np.array(rel_entropies),
             'delta_entropy': np.array(delta_entropies)}

        y = np.array(labels)

        test_input_fn = tf.estimator.inputs.numpy_input_fn(x, y, num_epochs=1, shuffle=False)

        # Evaluate accuracy.
        accuracy_score = self.classifier.evaluate(input_fn=test_input_fn)["accuracy"]

        print("\nTest Accuracy: {0:f}\n".format(accuracy_score))

    def predict_class(self, msgs):
        """
        Uses DNN Classifier to predict message class

        :param msgs: list of features corresponding to each message in the form
        [[Message ID, Occurrences in last second, Relative Entropy, Change in System Entropy]]
        :return: list with class predictions
        """
        msg_ids = []
        occurrences = []
        rel_entropies = []
        delta_entropies = []
        for msg in msgs:
            msg_ids.append(msg[0])
            occurrences.append(msg[1])
            rel_entropies.append(msg[2])
            delta_entropies.append(msg[3])

        x = {'msg_id': np.array(msg_ids),
             'occurrences_in_last_s': np.array(occurrences),
             'relative_entropy': np.array(rel_entropies),
             'delta_entropy': np.array(delta_entropies)}

        predict_input_fn = tf.estimator.inputs.numpy_input_fn(x, num_epochs=1, shuffle=False)

        predictions = list(self.classifier.predict(input_fn=predict_input_fn))
        return [p["classes"] for p in predictions]
