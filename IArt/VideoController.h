#pragma once

#include"Util.h"

#define ANIMATION_NUM 1

const Size VIDEO_SIZE = Size(640, 480);

class VideoPlayer
{
public:
	VideoPlayer() {}
	VideoPlayer(Point point, String videoName) : mPoint(point)
	{
		mVideoCapture = VideoCapture(videoName);
		if (!mVideoCapture.isOpened())
		{
			cout << "open video failed" << endl;
		}
	}

	void getFrame()
	{
		if (!isPlaying)
			return;

		if (!mVideoCapture.read(frame))
		{
			isPlaying = false;
			mVideoCapture.set(CAP_PROP_POS_FRAMES, 0);
			return;
		}

		resize(frame, frame, VIDEO_SIZE);
	}

	VideoCapture mVideoCapture;
	bool isPlaying = false;
	Point mPoint;
	Mat frame;

private:

};


//单例
//负责给出所有视频叠加后的图像
class VideoController
{
public:
	VideoController();
	~VideoController();

	static VideoController* getVideoController();

	void play();

	Mat mDepthFrame;

private:
	bool judge();

	static VideoController *instance;

	Mat mBackground;

	VideoPlayer mVideoPlayer[ANIMATION_NUM];
	Mat mMixFrame;
	Mat tmpBackGroundINV;
	Mat tmpGrayMixFrame;
	Mat tmpBackGround;
	Mat tmpAlpha;
	Mat tmpGrayBackground;
	Mat tmpDepthFrame;
};



