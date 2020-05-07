# 将自己制作的图片放到文件夹中统一转换成csv可以用的格式
import os
import scipy.misc
import numpy
import matplotlib.pyplot

import imageio

# 这里放着你要操作的文件夹名称
path = "E:\\progect_workspace\\github\\my_server\\deep_study\\tensorflow\\my_data\\my_pic\\"

# 把D:\\vsc_pro\\CNN\\data\\3\\目录下的文件名全部获取保存在files中

files = os.listdir(path)

# 存放写入文件的内容
file_array = []
for file in files:
    # 从常见的图像文件格式（包括PNG和JPG）中读取和解码数据
    # flatten是是否将图片变成简单的浮点数组，如果图片是彩色的，就转换成灰度
    # scipy库版本太高，就没有imread函数。需要回退版本到1.2.1.  pip install scipy==1.2.1
    #img_data_temp = scipy.misc.imread(path + file, flatten=True)
    img_data_temp = imageio.imread(path+file, as_gray=True)
    # 常规而言，0是黑色，255是白色，但是MNIST数据集用相反的方式表示，所以要逆转过来
    img_data = 255.0-img_data_temp
    # 将从文件中读出的28*28的数据变成一行784个数
    img_array = img_data.reshape(784)
    # 将数据转换成list，因为list是可变长度的，才能insert，numpy的list都是固定长度的
    img_array = img_array.tolist()
    # 获取文件路径，文件名，文件后缀
    file_name, file_ext = os.path.splitext(file)
    # 给数据添加标签
    img_array.insert(0, int(file_name))
    # 添加一个数据
    file_array.append(img_array)

print(file_array)
# 把数据转换成numpy的list，因为下面的savatxt的参数是numpy的list
file_array = numpy.array(file_array)
# 写文件，savetxt是numpy提供的函数，第一个参数是文件名，第二个参数是写入的内容
# 第三个参数是对写入的内容进行分割，第四个参数是一行多长，第五个是用什么分隔每行
numpy.savetxt("my_data_train.csv", file_array, delimiter=',', fmt=['%s']*file_array.shape[1], newline='\n')