#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
 
int main()
{
    Mat image = imread("/home/hao/Pictures/Aurora.jpg", 1);
    if (image.empty())
    {
        std::cout << "Image file is empty." << std::endl;
        return -1;
    }
 
    namedWindow("Test");
    imshow("Test", image);
 
    waitKey(0);
 
    return 0;
}
