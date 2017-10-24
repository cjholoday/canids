import tensorflow as tf


temp_num_features = 4

class SimpleNN:
    """Class for neural network detector"""
    def __init__(self, features, labels):
        # Message ID, Occurrences in last second, Relative Entropy, Change in System Entropy
        self.features = tf.placeholder(tf.float32, [None, temp_num_features])

        self.W = tf.Variable(tf.zeros([temp_num_features, 2]))  # Weights
        self.b = tf.Variable(tf.zeros([2]))  # Bias

        self.labels = tf.nn.softmax(tf.matmul(self.features, self.W) + self.b)

        self.actual_labels = tf.placeholder(tf.float32, [None, 2])

        self.cross_entropy = tf.reduce_mean(-tf.reduce_sum(self.actual_labels
                                                           * tf.log(self.labels),
                                                           reduction_indices=[1]))

        self.train_step = tf.train.GradientDescentOptimizer(0.5).minimize(self.cross_entropy)

        self.sess = tf.InteractiveSession()
        tf.global_variables_initializer().run()

        self.sess.run(self.train_step, feed_dict={self.features: features,
                                                  self.actual_labels: labels})

        self.correct_prediction = tf.equal(tf.argmax(self.labels, 1),
                                           tf.argmax(self.actual_labels, 1))
        self.accuracy = tf.reduce_mean(tf.cast(self.correct_prediction, tf.float32))

        print(self.sess.run(self.accuracy, feed_dict={self.features: features,
                                                      self.actual_labels: labels}))

        self.saver = tf.train.Saver()
        self.saver.save(self.sess, 'simple_nn_model')
