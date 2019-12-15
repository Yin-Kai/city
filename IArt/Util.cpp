#include "Util.h"



Util::Util()
{
}


Util::~Util()
{
}

//print error and execute the function "fp"
void Util::getError(Status status,string msg,void (*fp)())
{
	if (status != STATUS_OK)
	{
		cout << msg << endl;
		fp();
	}
}

void Util::getError(Status status, string msg, VideoStream & stream)
{
	if (status != STATUS_OK)
	{
		cout << msg << endl;
		stream.destroy();
	}
}

void Util::getError(Status status, string msg)
{
	if (status != STATUS_OK)
	{
		cout << msg << endl;
	}
}

void Util::oni2Mat(VideoFrameRef &oniCv,Mat &dst)
{
	if (oniCv.getSensorType() == SENSOR_COLOR)
	{
		RGB888Pixel *dData = (RGB888Pixel*)oniCv.getData();
		dst = Mat(oniCv.getHeight(), oniCv.getWidth(), CV_8UC3, dData);
		cvtColor(dst, dst, COLOR_RGB2BGR);
	}
	else if (oniCv.getSensorType() == SENSOR_DEPTH)
	{
		DepthPixel *dData = (DepthPixel*)oniCv.getData();
		dst = Mat(oniCv.getHeight(), oniCv.getWidth(), CV_16UC1, dData);
	}
	else
	{
		throw runtime_error("Unsupported sensor type.");
	}
}

//均衡直方图同时转为每通道8位的图像
void Util::equalizeHist(Mat & src, Mat & dst)
{
	Point minLoc, maxLoc;
	UINT16 min, max;

	minMaxLoc(src, NULL, NULL, &minLoc, &maxLoc);
	min = src.at<UINT16>(minLoc);
	max = src.at<UINT16>(maxLoc);

	double scale = 255.0 / (max - min);

	convertScaleAbs(src, dst, scale);
}

//src1为三通道，src2为单通道，两者尺寸要一致
void Util::addWeighted(Mat& src1, Mat& src2, Mat& dst)
{
	int rows = src1.rows;
	int cols = src1.cols;

	for (size_t i = 0; i < rows; i++)
	{
		const uchar* inData1 = src1.ptr<uchar>(i);
		const float16_t* inData2 = src2.ptr<float16_t>(i);
		uchar* outData = dst.ptr<uchar>(i);

		for (size_t j = 0; j < cols; j++)
		{
			//src1三个通道都和src2的单通道相乘
			*outData++ = *inData1++ * *inData2;
			*outData++ = *inData1++ * *inData2;
			*outData++ = *inData1++ * *inData2;
			++inData2;
		}
	}
}






