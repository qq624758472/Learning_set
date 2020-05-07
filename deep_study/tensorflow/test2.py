# #%matplotlib inline
# import numpy
# import matplotlib.pyplot
#
# import numpy
# # scipy.special for the sigmoid function expit()
# import scipy.special
# import scipy.misc
# # library for plotting arrays
# import matplotlib.pyplot
#
# # helper to load data from PNG image files
# import imageio
# # glob helps select multiple files using patterns
# import glob
#
#
# class neuralNetwork:
#     # initialise the neural network
#     def __init__(self, inputnodes, hiddennodes, outputnodes, learningrate):
#         # set number of nodes in each input, hidden, output layer
#         self.inodes = inputnodes
#         self.hnodes = hiddennodes
#         self.onodes = outputnodes
#
#         # link weight matrices, wih and who
#         # weights inside the arrays are w_i_j, where link is from node i to node j in the next layer
#         # w11 w21
#         # w12 w22 etc
#         self.wih = numpy.random.normal(0.0, pow(self.inodes, -0.5), (self.hnodes, self.inodes))
#         self.who = numpy.random.normal(0.0, pow(self.hnodes, -0.5), (self.onodes, self.hnodes))
#
#         # learning rate
#         self.lr = learningrate
#
#         # activation function is the sigmoid function
#         self.activation_function = lambda x: scipy.special.expit(x)
#
#         pass
#
#     # train the neural network
#     def train(self, inputs_list, targets_list):
#         # convert inputs list to 2d array
#         inputs = numpy.array(inputs_list, ndmin=2).T
#         targets = numpy.array(targets_list, ndmin=2).T
#
#         # calculate signals into hidden layer
#         hidden_inputs = numpy.dot(self.wih, inputs)
#         # calculate the signals emerging from hidden layer
#         hidden_outputs = self.activation_function(hidden_inputs)
#
#         # calculate signals into final output layer
#         final_inputs = numpy.dot(self.who, hidden_outputs)
#         # calculate the signals emerging from final output layer
#         final_outputs = self.activation_function(final_inputs)
#
#         # output layer error is the (target - actual)
#         output_errors = targets - final_outputs
#         # hidden layer error is the output_errors, split by weights, recombined at hidden nodes
#         hidden_errors = numpy.dot(self.who.T, output_errors)
#
#         # update the weights for the links between the hidden and output layers
#         self.who += self.lr * numpy.dot((output_errors * final_outputs * (1.0 - final_outputs)),
#                                         numpy.transpose(hidden_outputs))
#
#         # update the weights for the links between the input and hidden layers
#         self.wih += self.lr * numpy.dot((hidden_errors * hidden_outputs * (1.0 - hidden_outputs)),
#                                         numpy.transpose(inputs))
#
#         pass
#
#     # query the neural network
#     def query(self, inputs_list):
#         # convert inputs list to 2d array
#         inputs = numpy.array(inputs_list, ndmin=2).T
#
#         # calculate signals into hidden layer
#         hidden_inputs = numpy.dot(self.wih, inputs)
#         # calculate the signals emerging from hidden layer
#         hidden_outputs = self.activation_function(hidden_inputs)
#
#         # calculate signals into final output layer
#         final_inputs = numpy.dot(self.who, hidden_outputs)
#         # calculate the signals emerging from final output layer
#         final_outputs = self.activation_function(final_inputs)
#
#         return final_outputs
#
#
#
#
# # data_file = open("my_data/mnist_train_100.csv", 'r')
# # data_list = data_file.readlines()
# # data_file.close()
# #
# # print (data_list)
# #
# # all_values= data_list[0].split(',')
# # print (all_values)
# # image_array= numpy.asfarray( all_values[1:]).reshape((28,28))
# # print(image_array)
# # matplotlib.pyplot.imshow( image_array, cmap='Greys',interpolation='None')
# # matplotlib.pyplot.colorbar()
# # matplotlib.pyplot.show()
#
# # number of input, hidden and output nodes
# input_nodes = 784
# hidden_nodes = 200
# output_nodes = 10
#
# # learning rate
# learning_rate = 0.1
#
# # create instance of neural network
# n = neuralNetwork(input_nodes,hidden_nodes,output_nodes, learning_rate)
#
#
# # load the mnist training data CSV file into a list
# training_data_file = open("my_data/mnist_test_10.csv", 'r')
# training_data_list = training_data_file.readlines()
# training_data_file.close()
#
# # train the neural network
#
# # epochs is the number of times the training data set is used for training     epochs是训练数据集用于训练的次数
# epochs = 10
#
# for e in range(epochs):
#     # go through all records in the training data set
#     for record in training_data_list:
#         # split the record by the ',' commas
#         all_values = record.split(',')
#         # scale and shift the inputs  缩放和移动输入
#         inputs = (numpy.asfarray(all_values[1:]) / 255.0 * 0.99) + 0.01
#         # create the target output values (all 0.01, except the desired label which is 0.99)   创建目标输出值（全部为0.01，所需标签为0.99）
#         targets = numpy.zeros(output_nodes) + 0.01
#         # all_values[0] is the target label for this record   all_values [0]是此记录的目标标签
#         targets[int(all_values[0])] = 0.99
#         n.train(inputs, targets)
#         pass
#     pass
#
# # our own image test data set
# our_own_dataset = []
#
# # load the png image data as test data set
# for image_file_name in glob.glob('my_data/2.png'):
#     # use the filename to set the correct label
#     label = int(image_file_name[-5:-4])
#
#     # load image data from png files into an array
#     print("loading ... ", image_file_name)
#     img_array = imageio.imread(image_file_name, as_gray=True)
#
#     # reshape from 28x28 to list of 784 values, invert values        从28x28整形为784个值列表，取反值
#     img_data = 255.0 - img_array.reshape(784)
#
#     # then scale data to range from 0.01 to 1.0
#     img_data = (img_data / 255.0 * 0.99) + 0.01
#     print(numpy.min(img_data))
#     print(numpy.max(img_data))
#
#     # append label and image data  to test data set
#     record = numpy.append(label, img_data)
#     our_own_dataset.append(record)
#
#     pass
#
# # test the neural network with our own images
#
# # record to test
# item = 2
#
# # plot image
# matplotlib.pyplot.imshow(our_own_dataset[item][1:].reshape(28,28), cmap='Greys', interpolation='None')
#
# # correct answer is first value
# correct_label = our_own_dataset[item][0]
# # data is remaining values
# inputs = our_own_dataset[item][1:]
#
# # query the network
# outputs = n.query(inputs)
# print (outputs)
#
# # the index of the highest value corresponds to the label
# label = numpy.argmax(outputs)
# print("network says ", label)
# # append correct or incorrect to list
# if (label == correct_label):
#     print ("match!")
# else:
#     print ("no match!")
#     pass




###############################################################
#######################上一部分神经网络代码####################
###############################################################
# make your own neural network
# code for a 3-layer neural network, and code for learning the MNIST dataset
import numpy
# scipy.special for the sigmoid function expit()
import scipy.special
# library for plotting arrays
import matplotlib.pyplot
import imageio
import os


global FLAG
FLAG = False
# neural network class definition
class neuralNetwork:

    # initialise the neural network
    # 初始化神经网络
    # inputnodes,hiddennodes,outputnodes分别是输入层，隐藏层和输出层网络节点的个数
    # learningrate是学习率
    def __init__(self, inputnodes, hiddennodes, outputnodes, learningrate):

        # set number of nodes in each input, hidden, output layer
        # 设置输入，隐藏和输出层节点的数量
        self.inodes = inputnodes
        self.hnodes = hiddennodes
        self.onodes = outputnodes

        # link weight matrices, wih an who
        # 链接权重矩阵
        # wih是输入层和隐藏层之间的链接权重矩阵W_input_hidden
        # who是输入层和隐藏层之间的链接权重矩阵W_hidden_output
        # weights inside the arrays are w_i_j, where link is from node i to node j in the next layer
        # 数组里的权重是wij，其中链接是从节点i到节点j的下一层
        # w11 w21
        # numpy.random.normal(a,b,(X,Y))的意思是生成一个随机数组,数组大小为X*Y，内容服从中心值为a，方差为b
        self.wih = numpy.random.normal(
            0.0, pow(self.inodes, -0.5), (self.hnodes, self.inodes))
        self.who = numpy.random.normal(
            0.0, pow(self.inodes, -0.5), (self.onodes, self.hnodes))

        # learning rate
        # 设置学习率
        self.lr = learningrate
        # activation function is the sigmod function
        self.activation_function = lambda x: scipy.special.expit(x)
        pass

    # train the neural network
    def train(self, inputs_list, targets_list):
        # convert inputs list to 2d array
        inputs = numpy.array(inputs_list, ndmin=2).T
        targets = numpy.array(targets_list, ndmin=2).T

        # calculate signals into hidden layer
        hidden_inputs = numpy.dot(self.wih, inputs)
        # calculate the signals emerging from hidden layer
        hidden_outputs = self.activation_function(hidden_inputs)

        # calculate signals into final output layer
        final_inputs = numpy.dot(self.who, hidden_outputs)
        # calculate the signals emerging from final output layer
        final_outputs = self.activation_function(final_inputs)

        # output layer error is the (target-actual)
        output_errors = targets - final_outputs
        # hidden layer error is the output_errors,split by weights,recombined at hidden nodes
        hidden_errors = numpy.dot(self.who.T, output_errors)

        # update the weights for the links between the hidden and output layers
        self.who += self.lr * numpy.dot((output_errors * final_outputs * (1.0 - final_outputs)),
                                        numpy.transpose(hidden_outputs))

        # update the weights for the links between the input and hidden layers
        self.wih += self.lr * numpy.dot((hidden_errors * hidden_outputs * (1.0 - hidden_outputs)),
                                        numpy.transpose(inputs))

        pass

    # query the neural network
    def query(self, inputs_list):
        # convert inputs list to 2d array
        inputs = numpy.array(inputs_list, ndmin=2).T
        # calculate signals into hidden layer
        # 计算隐藏层的输入
        # numpy.dot(X,Y)的意思是两个数组的点乘
        hidden_inputs = numpy.dot(self.wih, inputs)
        # calculate the signals emerging from hidden layer
        # 计算隐藏层的输出
        hidden_outputs = self.activation_function(hidden_inputs)
        # calculate signals into final output layer
        # 计算输出层的输入
        final_inputs = numpy.dot(self.who, hidden_outputs)
        # calculate the signals emerging from final output layer
        # 计算输出层的输出
        final_outputs = self.activation_function(final_inputs)
        return final_outputs

# number of input, hidden and output nodes
# 设置输入，隐藏和输出层节点的数量
# 输出层有28*28=784个数据
input_nodes = 784
# 这个自己随便设的
hidden_nodes = 200
# 按本例子的的方案，输出有10中，结点有10个
output_nodes = 10

# learning rate is 0.3
# 设置学习率为0.3
learning_rate = 0.3

# create instance of neural network
# 创建一个神经网络的实例
n = neuralNetwork(input_nodes, hidden_nodes, output_nodes, learning_rate)

# load the mnist training data CSV file into a list
# 训练集有6万条，测试集有1万条
# 读文件，open函数的参数列表，第一个是要打开的文件，第二个是打开方式，这里r代表只读方式
training_data_file = open("my_data/mnist_train.csv", 'r')
# readlines()函数的意思是读取整个文件所有的内容，生成一个字符串数组，每行是一个字符串
training_data_list = training_data_file.readlines()
# 打开的文件必须close()掉，否则可能会出现各种问题。
training_data_file.close()

# train the neural network
# go through all recordes in the training data set
for record in training_data_list:
    # split the record by the ',' commas
    # 用逗号分隔字符串，将长的文本字符串拆分成单个的值
    training_all_values = record.split(',')
    # scale and shift the inputs
    # 整理输入数据
    training_inputs = (numpy.asfarray(training_all_values[1:])/255.0*0.99)+0.01
    # PS：这里能不能不每次都设置新的target，待改进：提前设置好然后直接调用某个目标输出
    # create the target output values (all 0.01, except the desired label which is 0.09)
    # 生成一个长度为10的数组，然后都加0.01，这样就10个都是0.01了
    training_targets = numpy.zeros(output_nodes)+0.01
    # training_all_values[0] is the target label for this record
    # 然后再将标签所指的那个点设成0.99即可
    #print ('training_all_values[0]'+training_all_values[0])
    training_targets[int(training_all_values[0])] = 0.99
    # 开始训练
    n.train(training_inputs, training_targets)
    pass


if FLAG == True:
    # 测试部分代码
    # load the mnist test data CSV file into a list
    # 读测试数据文件
    test_data_file = open("my_data/my_data_train.csv", 'r')
    test_data_list = test_data_file.readlines()
    test_data_file.close()


    no = 0

    # test the neural network
    # scorecard for how well the network perform, initially empty
    scorecard = []
    # go through all the records in the test data set
    for record in test_data_list:
        # split the record by the ',' commas
        all_values = record.split(',')
        # correct answer is first value
        correct_label = int(all_values[0])
        print(correct_label, "correct label")
        # scale and shift the inputs
        inputs = (numpy.asfarray(all_values[1:])/255.0*0.99)+0.01
        # query the network
        outputs = n.query(inputs)
        # the index of the highest value corresponds to the label
        label = numpy.argmax(outputs)
        print(label, "network's answer")
        # append correct or incorrect to list
        if (label == correct_label):
            # network's answer matches correct answer, add 1 to scorecard
            scorecard.append(1)
        else:
            # network's answer doesn't match correct answer, add O to scorecard
            scorecard.append(0)
            print("第" + str(no) + "号不对")
            print(correct_label, "correct label")
            print(label, "network's answer")
            image_array = numpy.asfarray(all_values[1:]).reshape((28, 28))
            matplotlib.pyplot.imshow( image_array, cmap='Greys', interpolation='None')
            matplotlib.pyplot.show()
            pass
        no += 1
        pass
    # calculate the performance score, the fraction of correct answers
    scorecard_array = numpy.asarray(scorecard)
    print("performance = ", scorecard_array.sum()/scorecard_array.size)
else:
    #需要预测的图片的地址
    image_file_name = "E:\\progect_workspace\\github\\my_server\\deep_study\\tensorflow\\my_data\\my_pic\\6.png"
    img_array = imageio.imread(image_file_name, as_gray=True)
    img_data = 255.0 - img_array.reshape(784)
    img_data = (img_data / 255.0 * 0.99) + 0.01
    print(numpy.min(img_data))
    print(numpy.max(img_data))

    # 显示上面的画的图像
    # matplotlib.pyplot.imshow(img_array, cmap='Greys', interpolation='None')
    # matplotlib.pyplot.show()

    outputs = n.query(img_data)
    label = numpy.argmax(outputs)
    print(outputs)
    print(label)

