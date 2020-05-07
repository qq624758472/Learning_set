#include "pch.h"
#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>    
#include <opencv2/highgui/highgui.hpp>   
extern "C"
{
#include <libavformat/avformat.h>    
#include <libavcodec/avcodec.h>    
#include <libswscale/swscale.h>     
#include <libavutil/opt.h>  

#include <libavutil/channel_layout.h>  
#include <libavutil/common.h>  
#include <libavutil/imgutils.h>  
#include <libavutil/mathematics.h>  
#include <libavutil/samplefmt.h>
}

#include <windows.h>
#include "ffmpeg_test.h"

using namespace std;
using namespace cv;
int ffmpeg_test()
{
	int result = 0;
	av_register_all(); // 注册所有FFmpeg库所支持的文件格式和codec  
	avformat_network_init();

    AVFormatContext* pFormatCtx = NULL;  
    //const char* filename = "E://2.mp4"; //输入文件名  
    const char* filename = "rtsp://admin:a1234567@192.168.1.48:554"; 
  
    //设置接收数据为tcp
	AVDictionary *optionsDict = NULL;
	av_dict_set(&optionsDict, "rtsp_transport", "tcp",0);		//采用tcp传输

	// step1: 打开媒体文件,最后2个参数是用来指定文件格式，buffer大小和格式参数，设置成NULL的话，libavformat库会自动去探测它们  
	result = avformat_open_input(&pFormatCtx, filename, NULL, &optionsDict);
	if (result != 0)
	{
		cout << "open file fail" << endl;
		return -1;
	}
	cout << "open file succ" << endl;

	// step2: 查找信息流的信息  
	result = avformat_find_stream_info(pFormatCtx, NULL);
	if (result != 0)
	{
		cout << "find stream fail" << endl;
		return -1;
	}
	cout << "find stream succ" << endl;

	// step3: 打印信息  
	//av_dump_format(pFormatCtx, 0, filename, 0);  

	// step4：找到video流数据  
	int i = 0;
	int videoStream = -1;
	AVCodecContext* pCodecCtx = NULL;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}

	if (videoStream == -1)
	{
		cout << "find stream video fail" << endl;
		return -1;
	}
	cout << "find stream video succ." << endl;

	// 得到video编码格式  
	pCodecCtx = pFormatCtx->streams[videoStream]->codec;

	// step5: 得到解码器  
	AVCodec* pCodec = NULL;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	//pCodec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO); 
	if (pCodec == NULL)
	{
		cout << "find decoder fail" << endl;
		return -1;
	}
	cout << "find decoder succ" << endl;

	result = avcodec_open2(pCodecCtx, pCodec, NULL);
	if (result != 0)
	{
		cout << "open codec fail" << endl;
		return -1;
	}
	cout << "open codec succ" << endl;

	// step6: 申请原始数据帧 和 RGB帧内存  
	AVFrame* pFrame = NULL;
	AVFrame* pFrameRGB = NULL;
	pFrame = av_frame_alloc();
	pFrameRGB = av_frame_alloc();
	if (pFrame == NULL || pFrameRGB == NULL)
	{
		return -1;
	}

	int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
	cout << "pCodecCtx->width: " << pCodecCtx->width << "\npCodecCtx->height: " << pCodecCtx->height << "\nnumBytes is :" << numBytes << endl;
	uint8_t* buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
	avpicture_fill((AVPicture*)pFrameRGB, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

	int frameFinishsed = 0;
	AVPacket packet;
	i = 0;

#if 1
	SwsContext * pSwxCtx = sws_getContext(
		pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_BGR24,
		SWS_BILINEAR, 0, 0, 0);
#else
	cout << "pCodecCtx->width: " << pCodecCtx->width << "\npCodecCtx->height: " << pCodecCtx->height << endl;
	struct SwsContext * pSwxCtx = NULL;
#endif
	cv::Mat image;//(pCodecCtx->height, pCodecCtx->width, CV_8UC3); 
	int b = 0;
	int g = 1;
	int r = 2;
	i = 0;

	while (true)
	{
		// 得到数据包  
		result = av_read_frame(pFormatCtx, &packet);
		if (result != 0)
		{
			cout << "av_read_frame over errorcode : " << result << endl;
			break;
		}
		if (packet.stream_index == videoStream)
		{
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinishsed, &packet);
#if 0
			if (frameFinishsed) {
				// pSwxCtx = sws_getCachedContext(
				// 	pSwxCtx,
				// 	pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,   //始数据的宽高和原始像素格式(YUV420)
				// 	//目标宽，目标高，目标的像素格式(这里的宽高可能是手机屏幕分辨率，RGBA8888)，这里不仅仅包含了尺寸的转换和像素格式的转换
				// 	pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_BGR24,
				// 	SWS_BICUBIC,// flag 提供了一系列的算法，快速线性，差值，矩阵，不同的算法性能也不同，快速线性算法性能相对较高。只针对尺寸的变换。对像素格式转换无此问题
				// 	NULL, NULL, NULL);
				sws_scale(pSwxCtx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameRGB->data, pFrameRGB->linesize);
				for (int m = 0; m < pCodecCtx->height; m++)
				{
					for (int n = 0; n < pCodecCtx->width; n++)
					{
						image.at<Vec3b>(m, n)[r] = pFrameRGB->data[0][3 * (m*pCodecCtx->width + n) + 0];
						image.at<Vec3b>(m, n)[g] = pFrameRGB->data[0][3 * (m*pCodecCtx->width + n) + 1];
						image.at<Vec3b>(m, n)[b] = pFrameRGB->data[0][3 * (m*pCodecCtx->width + n) + 2];
					}
				}
				cout << "have a alame" << endl;
				cv::imwrite("pic.jpg", image);
				sleep(3);
#else
			if (frameFinishsed) {

				sws_scale(pSwxCtx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameRGB->data, pFrameRGB->linesize);

				Mat frametemp(pCodecCtx->height, pCodecCtx->width, CV_8UC3, (unsigned char *)pFrameRGB->data[0]);
				image = frametemp.clone();
				frametemp.release();
				//cout << "have a alame" << endl;
				////////////////////////////////////////////////
				//std::vector< int > compression_params;
				//compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);  //选择jpeg
				//compression_params.push_back(40); //在这个填入你要的图片质量
				///////////////////////////////////////////////

				cv::imshow("pic", image);
				//cv::imwrite("pic.jpg", image, compression_params);
				//Sleep(1);
#endif

			}
		}

		av_free_packet(&packet);
	}

	avformat_close_input(&pFormatCtx);

	//system("pause");  
	return 0;
}