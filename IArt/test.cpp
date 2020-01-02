#include"Util.h"
#include"View.h"
#include"VideoController.h"

bool flag = false;

void mouseCallback(int mouseEvent, int x, int y, int flags, void* param) {
	switch (mouseEvent) {
	case EVENT_LBUTTONDOWN:
		flag = true;
		break;
	case EVENT_MOUSEMOVE:
		if (flag)
			VideoController::getVideoController()->setXY(x, y);
		break;
	case EVENT_LBUTTONUP:
		flag = false;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	namedWindow("test");

	setMouseCallback("test", mouseCallback);

	View view;
	view.init();
	view.display();

	return 0;
}



/*	astra's API

	astra::initialize();

	astra::StreamSet streamSet;
	astra::StreamReader reader = streamSet.create_reader();

	reader.stream<astra::DepthStream>().start();

	const int maxFramesToProcess = 100;
	int count = 0;

	do
	{
		astra::Frame frame = reader.get_latest_frame();
		const auto depthFrame = frame.get<astra::DepthFrame>();

		const int frameIndex = depthFrame.frame_index();
		const short pixelValue = depthFrame.data()[0];

		std::cout << std::endl
			<< "Depth frameIndex: " << frameIndex
			<< " pixelValue: " << pixelValue
			<< std::endl
			<< std::endl;

		count++;
	} while (count < maxFramesToProcess);

	std::cout << "Press any key to continue...";
	std::cin.get();

	astra::terminate();

	std::cout << "hit enter to exit program" << std::endl;
	std::cin.get();

*/