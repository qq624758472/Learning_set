
#include "test_opencv.h"
int test()
{
    Mat image = imread("/home/hao/Pictures/Aurora.jpg", 1);
    if (image.empty())
    {
        std::cout << "Image file is empty." << std::endl;
        return -1;
    }
 
    namedWindow("Test");
    //imshow("Test", image);
 
    waitKey(0);
 
    return 0;
}
