#pragma once

#include"Util.h"


class View
{
public:
	View();
	~View();

	void init();
	void display();
	void setVideoMode(int fps, PixelFormat format, int x, int y);

	Mat mVieoFrame;

protected:
	Status mStatus;
	Device mDevice;
	VideoStream mColorStream;
	VideoStream mDepthStream;
	VideoStream **mStreams;
	VideoFrameRef mniColorFrame;
	VideoFrameRef mniDepthFrame;
	VideoCapture mVideoCapture;



private:
	Mat mcvColorFrame;
	Mat mcvDepthFrame;
	Mat mDepthGrayFrame;
	int mWidth;
	int mHeight;
	const char *DepthWindow = "depth";
	const char *ColorWindow = "color";

};

