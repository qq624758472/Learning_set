#pragma once
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


class OpencvFilter 
{
public:
	Mat mat1;
	Mat mat2;

	OpencvFilter(string path);

	bool box_filter();

};