#pragma once
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





class IdentificateFace
{
public:

	const string model_path = "E:\\work\\opencv\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	const string pic_path = "E:\\1.jpg";


	IdentificateFace();
	~IdentificateFace();


public:
	int face_identify();
	int sport_identify();
};

