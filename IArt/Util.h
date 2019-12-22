#pragma once

#include<iostream>
#include<string>

#include<OpenNI.h>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>



using namespace std;
using namespace openni;
using namespace cv;

class Util
{
public:
	Util();
	~Util();

	static void init();
	static void getError(Status status,string msg, void(*fp)());
	static void getError(Status status, string msg, VideoStream &stream);
	static void getError(Status status, string msg);
	static void oni2Mat(VideoFrameRef &oniCv, Mat &dst);
	static void equalizeHist(Mat &src, Mat &dst);
	static void addWeighted(Mat& src1, Mat& src2, Mat& dst);
	static void mix(Mat& src1, Mat& src2, Mat& dst);
	static void erase(Mat& src,Mat& dst, int x, int y);

	static Mat brush;					//单通道灰度图
};

