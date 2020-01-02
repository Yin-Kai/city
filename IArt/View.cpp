#include "View.h"
#include"VideoController.h"

#define VIEW_TIMEOUT 2000

//摄像头高度，单位mm
#define CAPTURE_HEIGHT 1000


class NIColorStreamCallback : public VideoStream::NewFrameListener
{
public:
	void onNewFrame(VideoStream &stream)
	{
		stream.readFrame(&mniFrame);
		Util::oni2Mat(mniFrame, mcvFrame);
		imshow("color", mcvFrame);
	}

private:
	VideoFrameRef mniFrame;
	Mat mcvFrame;
};

class NIDepthStreamCallback : public VideoStream::NewFrameListener
{
public:
	NIDepthStreamCallback()
	{
	}

	void onNewFrame(VideoStream &stream)
	{
		stream.readFrame(&mniFrame);
		Util::oni2Mat(mniFrame, mcvFrame);

		//深度转换为高度
		//mcvFrame = CAPTURE_HEIGHT - mcvFrame;

		//除去过高、过低的值
		//threshold(mcvFrame, mcvFrame, threshold_min_value, 255, THRESH_TOZERO);
		//threshold(mcvFrame, mcvFrame, threshold_max_value, 255, THRESH_TOZERO_INV);

		////mController->mDepthFrame = mcvFrame;

		//开操作
		////morphologyEx(mcvFrame, mController->mDepthFrame, MORPH_OPEN, Mat(),Point(-1,-1),2);

		//膨胀
		//dilate(mcvFrame, mController->mDepthFrame, Mat(), Point(-1, -1), 2);

		mController->play();
	}
private:
	//高度，单位mm
	const int threshold_min_value = 150;
	const int threshold_max_value = 3000;

	VideoController *mController = VideoController::getVideoController();
	VideoFrameRef mniFrame;
	Mat mcvMask;
	Mat mcvFrame;
	Mat mVieoFrame;
};


View::View()
{
}

View::~View()
{
	destroyAllWindows();

	if (mStreams != NULL)
	{
		delete[]mStreams;
	}

	if (mColorStream.isValid())
	{
		mColorStream.stop();
		mColorStream.destroy();
	}

	if (mDepthStream.isValid())
	{
		mDepthStream.stop();
		mDepthStream.destroy();
	}

	if (mVideoCapture.isOpened())
	{
		//mVideoCapture.release();
	}

	if (mDevice.isValid())
	{
		mDevice.close();
		OpenNI::shutdown();
	}
}

void View::init()
{
	mStatus = STATUS_OK;

	mStatus = OpenNI::initialize();
	Util::getError(mStatus, "initialize failed");

	mStatus = mDevice.open(ANY_DEVICE);
	Util::getError(mStatus, "device open failed", OpenNI::shutdown);

	//get mColorStream stream
	mStatus = mColorStream.create(mDevice, SENSOR_COLOR);
	Util::getError(mStatus, "mColorStream create failed");

	mStatus = mColorStream.start();
	Util::getError(mStatus, "mColorStream start failed", mColorStream);

	if (!mColorStream.isValid())
	{
		cout << "no valid mColorStream stream. Exiting" << endl;
		OpenNI::shutdown();
	}

	//get mDepthStream stream
	mStatus = mDepthStream.create(mDevice, SENSOR_DEPTH);
	Util::getError(mStatus, "mDepthStream create failed");

	mStatus = mDepthStream.start();
	Util::getError(mStatus, "mDepthStream start failed", mDepthStream);

	if (!mDepthStream.isValid())
	{
		cout << "no valid streams. Exiting" << endl;
		OpenNI::shutdown();
	}

	//get sensor type
	if (!mDevice.hasSensor(SENSOR_COLOR))
	{
		cout << "no mColorStream sensor" << endl;
	}

	if (!mDevice.hasSensor(SENSOR_DEPTH))
	{
		cout << "no mDepthStream sensor" << endl;
	}

	if (!mDevice.hasSensor(SENSOR_IR))
	{
		cout << "no IR sensor" << endl;
	}

	//init
	//mStreams = new VideoStream*;
	//mStreams[0] = &mColorStream;
	//mStreams[0] = &mDepthStream;

	//mVideoCapture = VideoCapture(0);
	//if (!mVideoCapture.isOpened())
	//{
	//	cout << "capture open failed" << endl;
	//}

	VideoMode depthMode = mDepthStream.getVideoMode();
	VideoMode colorMode = mColorStream.getVideoMode();

	int depthWidth = depthMode.getResolutionX();
	int depthHeight = depthMode.getResolutionY();

	int colorWidth = colorMode.getResolutionX();
	int colorHeight = colorMode.getResolutionY();

	if (depthWidth == colorWidth && depthHeight == colorHeight)
	{
		mWidth = depthWidth;
		mHeight = depthHeight;
	}
	else
	{
		cout << "depthSize isn't equals colorSize" << endl;
		cout << "depthWidth:" << depthWidth << "colorWidth:" << colorWidth << endl;
		cout << "depthHeight:" << depthHeight << "colorHeight:" << colorHeight << endl;
	}

	mDepthStream.setMirroringEnabled(false);
}

void View::display()
{
	//namedWindow(DepthWindow);
	//namedWindow(ColorWindow);

	//namedWindow("depth");

	char key = 0;

	NIDepthStreamCallback depthListener;
	mDepthStream.addNewFrameListener(&depthListener);

	//NIColorStreamCallback colorListener;
	//mColorStream.addNewFrameListener(&colorListener);


	while (key != 27)
	{
#pragma region polling

		//int streamIndex;

		//OpenNI::waitForAnyStream(mStreams, 1, &streamIndex, VIEW_TIMEOUT);
		//switch (streamIndex)
		//{
		//case 1:
		//	if (STATUS_OK != mColorStream.readFrame(&mniColorFrame))
		//	{
		//		cout << "readFrame failed" << endl;
		//	}
		//	mcvColorFrame = Util::oni2Mat(mniColorFrame);
		//	imshow(ColorWindow, mcvColorFrame);
		//	break;
		//case 0:
		//	mDepthStream.readFrame(&mniDepthFrame);
		//	mcvDepthFrame = Util::oni2Mat(mniDepthFrame);
		//	//equalizeHist(mcvDepthFrame, mcvDepthFrame);
		//	Util::depth2Gray(mcvDepthFrame, mDepthGrayFrame);
		//	imshow("depth", mDepthGrayFrame);
		//	break;
		//default:
		//	cout << "waitForAnyStream error" << endl;
		//	break;
		//}
#pragma endregion

#pragma region cvVideoCapture

		//mVideoCapture >> mcvColorFrame;
		//if (mcvColorFrame.cols > 0 && mcvColorFrame.rows > 0)
		//{
		//	imshow(ColorWindow, mcvColorFrame);
		//}
#pragma endregion

		key = waitKey(30);

		//Sleep(50);
	}

	//mColorStream.removeNewFrameListener(&colorListener);
	mDepthStream.removeNewFrameListener(&depthListener);

	//realse
	//destroyAllWindows();
	//mColorStream.destroy();
	//mDepthStream.destroy();
	//mVideoCapture.release();
	//mDevice.close();
	//OpenNI::shutdown();

}

void View::setVideoMode(int fps, PixelFormat format, int x, int y)
{
	VideoMode mode;
	mode.setFps(fps);
	mode.setPixelFormat(format);
	mode.setResolution(x, y);
	mColorStream.setVideoMode(mode);
	mDepthStream.setVideoMode(mode);

	mWidth = x;
	mHeight = y;
}


