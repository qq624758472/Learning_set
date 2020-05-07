
#ifndef TEST
#define TEST

#include"pch.h"
#include <stdio.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp>

#include<opencv2/video/video.hpp>

#include"opencv2/highgui/highgui.hpp"

#include <string>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp> 

#include <time.h>
#include <vector>

using namespace cv;
using namespace std;


//关于显示图像的类
class display
{
public:
	VideoCapture cap;
	VideoWriter outputVideo;  //处理过后，输出视频流
	double fps;    //FPS是测量用于保存、显示动态视频的信息数量。通俗来讲就是指每秒变化的画面数。

	VideoWriter saveVideo;  //需要保存的视频

private:

public:
	display();
	//截取视频流指定区域
	int region_display();
	//实时显示视频流
	int realtime_display();
	//每隔一分钟保存10秒的视频
	int save_video();
	
};


//将视频处理成图片的类
class picture_my
{
public:
	cv::Mat mat1;
	cv::Mat mat2;
private:


protected:

public:
	picture_my();
	//将一个图片读出来，然后加上自定义字符串。
	int draw_text_to_picture();
	//从视频中数帧数， 在某一帧上写字并保存成图片。
	int draw_test(Mat Camera_CImg, char* cstr);
	//腐蚀图
	int fs_picture();
	//打印Mat类中的矩阵
	int value_Mat();
	//比较两个图片是否相同。
	bool cvMatEQ(const cv::Mat& data1, const cv::Mat& data2);
	//ROI使用测试
	int roi_test();
	int save_pic();
};

int comp_picture();   //比较两个图片是否相同

#endif
