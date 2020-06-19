#!/usr/bin/python
# -*- coding: UTF-8 -*-

'''This is pill'''

from PIL import Image,ImageDraw,ImageFont,ImageFilter,ImageColor
import random
import sys
#from Tkinter import *
from tkinter import *  #python3的导入方式

# try:
#     from tkinter import *
# except ImportError:  #Python 2.x
#     PythonVersion = 2
#     from Tkinter import *
#     # from tkFont import Font
#     # from ttk import *
#     # from tkMessageBox import *
#     import tkFileDialog
# else:  #Python 3.x
#     PythonVersion = 3
#     from tkinter.font import Font
#     from tkinter.ttk import *
#     from tkinter.messagebox import *


class Application(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.pack()
        self.createWidgets()

    def createWidgets(self):
        self.helloLabel = Label(self, text='Hello, world!')
        self.helloLabel.pack()
        self.quitButton = Button(self, text='Quit', command=self.quit)
        self.quitButton.pack()

class TestTk():


    def __init__(self):
        self.app = Application()

    def operating(self):
        # 设置窗口标题:
        self.app.master.title('Hello World')
        # 主消息循环:
        self.app.mainloop()



class VerificationCode():
    def __init__(self,path):
        self.path = path
    
    # 随机字母:
    @staticmethod
    def rndChar():
        return chr(random.randint(65, 90))

    # 随机颜色1:
    @staticmethod
    def rndColor():
        return (random.randint(64, 255), random.randint(64, 255), random.randint(64, 255))

    # 随机颜色2:
    @staticmethod
    def rndColor2():
        return (random.randint(32, 127), random.randint(32, 127), random.randint(32, 127))

    def opetating(self):
        # 240 x 60:
        width = 60 * 4
        height = 60
        image = Image.new('RGB', (width, height), (255, 255, 255))
        # 创建Font对象:
        font = ImageFont.truetype(self.path, 36)
        # 创建Draw对象:
        draw = ImageDraw.Draw(image)
        # 填充每个像素:
        for x in range(width):
            for y in range(height):
                draw.point((x, y), fill=VerificationCode.rndColor())
        # 输出文字:
        for t in range(4):
            draw.text((60 * t + 10, 10), VerificationCode.rndChar(), font=font, fill=VerificationCode.rndColor2())
        # 模糊:
        image = image.filter(ImageFilter.BLUR)
        image.save('code.jpg', 'jpeg')
            
    
class suofang():
    def __init__(self, path):
        # 打开一个jpg图像文件，注意是当前路径:
        self.im = Image.open(path)
    
    def thumbnail(self):
        # 获得图像尺寸:
        w, h = self.im.size
        print('Original image size: %sx%s' % (w, h))
        # 缩放到50%:
        self.im.thumbnail((w//2, h//2))
        print('Resize image to: %sx%s' % (w//2, h//2))
        # 把缩放后的图像用jpeg格式保存:
        self.im.save('thumbnail.jpg', 'jpeg')

    def filter(self):
        # 应用模糊滤镜:
        im2 = self.im.filter(ImageFilter.BLUR)
        im2.save('blur.jpg', 'jpeg')

#path = '/home/hao/Pictures/Sunset_of_the_Lake_Nam_by_Wang_Jinyu.jpg'
path = '/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf'

test = VerificationCode(path)
test.opetating()


test1 = TestTk()
test1.operating()