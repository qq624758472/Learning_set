# -*- coding: utf-8 -*-
# @Author: Xingmo
 
import cv2
import numpy as np
 
# 读取图片: cv2.imread(路径,num) 其中num=0，为灰度图像；num=1为彩图
img = cv2.imread('002.jpg',0)
 
# 创建窗口，cv2.namedWindow(窗口名)
cv2.namedWindow('image')
 
# 保存图片,cv2.imwrite(保存图片名,要保存的图片)
cv2.imwrite('002.jpg',img)
# 第三个参数针对特定的格式： 对于JPEG，其表示的是图像的质量，用0-100的整数表示(越高越清晰，压缩级别越低)，默认为95。  注意，cv2.IMWRITE_JPEG_QUALITY类型为Long，必须转换成int。
cv2.imwrite('003.jpg',img,[int(cv2.IMWRITE_JPEG_QUALITY), 10])
# 对于PNG，第三个参数表示的是压缩级别。cv2.IMWRITE_PNG_COMPRESSION，从0到9,压缩级别越高，图像尺寸越小。默认级别为3
cv2.imwrite('004.png', img, [int(cv2.IMWRITE_PNG_COMPRESSION), 5])
# 图片显示，cv2.imshow(窗口名，要显示的图片)
cv2.imshow('image1',img)
 
# 复制img图片
#emptyimage = img.copy()
# 创建空图片
emptyimage = np.zeros(img.shape,np.uint8)
cv2.imshow('image2',emptyimage)
 
# 键盘绑定函数
cv2.waitKey(0)
# 释放窗口
cv2.destroyAllWindows()