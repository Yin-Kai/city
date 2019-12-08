#include "VideoController.h"


#define MIN_HEIGHT 10

VideoController::VideoController()
{
	//初始化所有待触发的区域
	mVideoPlayer[0] = VideoPlayer(Point2i(230, 150), "res/1-1.avi");

	//初始化Mat
	mMixFrame = Mat::zeros(VIDEO_SIZE, CV_8UC3);
	//tmpDepthFrame = Mat(VIDEO_SIZE, CV_32FC3);

	mBackground = imread("res/background.jpg");
	if (mBackground.empty())
	{
		cout << "imread failed" << endl;
	}
	else
	{
		resize(mBackground, mBackground, VIDEO_SIZE);
	}
}

VideoController::~VideoController()
{
	for (size_t i = 0; i < ANIMATION_NUM; i++)
	{
		mVideoPlayer->mVideoCapture.release();
	}

	destroyAllWindows();
}

VideoController* VideoController::getVideoController()
{
	return instance;
}

void VideoController::play()
{
	tmpBackGround = mBackground.clone();

	if (judge())
	{
		mMixFrame = ~mMixFrame;

		//得到掩码
		cvtColor(mMixFrame, tmpGrayMixFrame, COLOR_BGR2GRAY);

		mMixFrame.copyTo(tmpBackGround, tmpGrayMixFrame);
	}
	
	//黑色遮罩
	Util::equalizeHist(mDepthFrame, mDepthFrame);
	normalize(mDepthFrame, tmpDepthFrame, 0.5, 1.0, NORM_MINMAX);
	//Util::equalizeHist(mDepthFrame, tmpDepthFrame);

	vector<Mat> alpha;
	alpha.push_back(tmpDepthFrame);
	alpha.push_back(tmpDepthFrame);
	alpha.push_back(tmpDepthFrame);
	merge(alpha, tmpAlpha);

	//convertScaleAbs(tmpDepthFrame, tmpDepthFrame, 255);
	//imshow("depth", tmpDepthFrame);//test

	//透明通道叠加
	//addWeighted(tmpBackGround, 0.3, tmpAlpha, 0.7, 0, tmpBackGround);

	multiply(tmpAlpha, tmpBackGround, tmpBackGround);

	imshow("test", tmpBackGround);
}

//判断是否触发动画
bool VideoController::judge()
{
	bool flag = false;

	for (size_t i = 0; i < ANIMATION_NUM; i++)
	{
		if (mDepthFrame.at<UINT16>(mVideoPlayer[i].mPoint) > MIN_HEIGHT && !mVideoPlayer[i].isPlaying)
			mVideoPlayer[i].isPlaying = true;

		mVideoPlayer[i].getFrame();

		if (mVideoPlayer[i].isPlaying)
		{
			mMixFrame += mVideoPlayer[i].frame;
			flag = true;
		}
	}

	return flag;
}

VideoController* VideoController::instance = new VideoController();