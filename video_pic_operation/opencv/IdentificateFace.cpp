#include "pch.h"
#include "IdentificateFace.h"


IdentificateFace::IdentificateFace()
{
}


IdentificateFace::~IdentificateFace()
{
}



//人脸识别操作
int IdentificateFace::face_identify()
{
	Mat img = imread(pic_path);
	//cout << img << endl;
	namedWindow("display");
	imshow("display", img);

	/*********************************** 1.加载人脸检测器  ******************************/
	// 建立级联分类器
	CascadeClassifier cascade;
	// 加载训练好的 人脸检测器（.xml）
	if (!cascade.load(model_path))
	{
		cout << "cascade load failed!\n";
		return 1;
	}

	//计时
	double t = 0;
	t = (double)getTickCount();
	/*********************************** 2.人脸检测 ******************************/
	vector<Rect> faces(0);
	cascade.detectMultiScale(img, faces, 1.1, 2, 0, Size(30, 30));

	cout << "detect face number is :" << faces.size() << endl;
	/********************************  3.显示人脸矩形框 ******************************/

	if (faces.size() > 0)
	{
		for (size_t i = 0; i < faces.size(); i++)
		{
			rectangle(img, faces[i], Scalar(150, 0, 0), 3, 8, 0);

		}
	}
	else cout << "未检测到人脸" << endl;

	t = (double)getTickCount() - t;  //getTickCount():  Returns the number of ticks per second.
	cout << "检测人脸用时：" << t * 1000 / getTickFrequency() << "ms (不计算加载模型的时间）" << endl;

	namedWindow("face_detect");
	imshow("face_detect", img);
	while (waitKey(0) != 'k');
	destroyWindow("display");
	destroyWindow("face_detect");
}


//人脸识别操作
//
//Ptr<backgroundsubtractormog2> cv::createBackgroundSubtractorMOG2(
//	int 	history = 500,
//	double 	varThreshold = 16,
//	bool 	detectShadows = true
//)
//Ptr<BackgroundSubtractorMOG2> mog = createBackgroundSubtractorMOG2(100,25,false);   //用于训练背景的帧数，默认为500帧






//动目标识别
int IdentificateFace::sport_identify()
{
	VideoCapture video("E://target_file_2.mp4");    //读取文件，注意路径格式，VideoCapture video(0); 表示使用编号为0的摄像头"11.mp4"
	int frameNum = 1;
	Mat frame, mask, thresholdImage, output;
	if (!video.isOpened()) {
		cout << "fail to open!" << endl;
		return -1;
	}
		
	double totalFrameNumber = video.get(CAP_PROP_FRAME_COUNT); //应该是得到视频总帧数
	video >> frame;
	Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();  //混合高斯分布（GMM）是背景建模中的经典算法
	
	bgsubtractor->setVarThreshold(20);  //// 模型匹配阈值
	/*bgsubtractor->getBackgroundImage(output);  //获取背景
	if (!output.empty()) {
		imshow("mask2", output);
	}*/
	
	
	while (true) {
		if (totalFrameNumber == frameNum)
			break;
		//if (frameNum % 2 == 0) continue;
		video >> frame;
		resize(frame, thresholdImage, cv::Size(1280/4, 720/4), 0, 0, cv::INTER_LINEAR);	//压缩图片
		cvtColor(thresholdImage, output, CV_BGR2GRAY); //转换BGR彩色图为灰度图。
		blur(output, output, Size(7, 7));			   //使用3×3内核将灰度图将噪。
		++frameNum;
		//bgSubtractor(frame, mask, 0.001);
		bgsubtractor->apply(output, mask, 0.0009);

		////初始化结果图
		//Mat dstImage = Mat::zeros(mask.rows, mask.cols, CV_8UC3);
		////frame 取阈值大于119的那部分。
		//mask = mask > 119;
		////imshow("取阈值后的原始图", mask);
		////定义轮廓和层次结构
		//vector<vector<Point>> contours;
		//vector<Vec4i> hierarchy;
		////查找轮廓
		//findContours(mask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		////遍历所有顶层的轮廓， 以随机颜色绘制出每个连接组件颜色。
		//int index = 0;
		//for (; index >= 0; index = hierarchy[index][0]) {
		//	Scalar color(rand() & 255, rand() & 255, rand() & 255);
		//	drawContours(dstImage, contours, index, color, FILLED, 8, hierarchy);
		//}
		
		imshow("mask", mask);
		waitKey(10);
	}
error:
	return -1;
success:
	return 1;
}