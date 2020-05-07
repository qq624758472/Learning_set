
#include "opencv_filter.h"
#include"pch.h"

OpencvFilter::OpencvFilter(string path)
{
	mat1 = cv::imread(path);
	if (mat1.empty()) {
		printf("Faild open file.");
	}
}



bool OpencvFilter::box_filter()
{
	if (mat1.empty()) {
		cout << "Error!" << endl;
		return false;
	}
	boxFilter(mat1, mat2, -1, Size(3, 3), Point(-1, -1), true, BORDER_DEFAULT);
	imshow("boxFilter1", mat1);
	//imshow("boxFilter2", mat2);
	if (cv::waitKey(1) == 'q') return true;
}