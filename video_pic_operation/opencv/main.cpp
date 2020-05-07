#include"pch.h"
#include "my_ffmpeg.h"
#include "opencv_test.h"
#include "IdentificateFace.h"

#include "myVideoCapture.h"   //视频写操作

#include "../opencv_filter.h"  //滤波操作，（卷积）

#define DEBUG

int main(int argc, char *argv[]) 
{
#ifdef DEBUG1
	ffmpeg test;
	test.display_ffmpeg();
#endif

#ifdef DEBUG2
	picture_my test;
	test.fs_picture();
#endif

#ifdef DEBUG3
	picture_my test;
	test.value_Mat();
#endif

#ifdef DEBUG4
	string a = "aaaaaaaaaaaaaaaaaaaaaaaaaaa";
	constexpr size_t len0 = sizeof("wired_init_req") - 1;
	
	cout << a.substr(2, len0);
#endif

#ifdef DEBUG5
	picture_my test;

	cout << test.cvMatEQ(test.mat1, test.mat2) << endl;
#endif


#ifdef DEBUG6
	comp_picture();
#endif

#ifdef DEBUG
	IdentificateFace test;
	test.face_identify();   //人脸识别
	test.sport_identify();		//动目标识别。
#endif

#ifdef DEBUG8
	display test;
	test.realtime_display();  //实时显示视频。
	
#endif

	//ffmpeg转码视频测试。
#ifdef DEBUG9
	ffmpeg test;
	test.display_my_ffmpeg();
#endif


#ifdef DEBUG10
	//ROI测试。
	picture_my test;
	//test.roi_test();
	test.save_pic();
#endif

#ifdef DEBUG11   
	auto t = [](string &t1, string &t2) {t1 = "liu"; t2 = "shi"; cout << "t1=" << t1 << ";t2=" << t2 << endl; };//匿名函数
	string t1;
	string t2;
	t(t1, t2);//引用传递
	cout << "line2:\nt1=" << t1 << ";t2=" << t2 << endl;
#endif

	//滤波器的使用
#ifdef DEBUG12
	OpencvFilter test("E:\\1.jpg");
	test.box_filter();
#endif
	getchar();
	return 0;
}