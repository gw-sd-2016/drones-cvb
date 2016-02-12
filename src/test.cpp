#include <cstdlib>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {

	int x = atoi(argv[1]);

	VideoCapture camera(x);

	if (!camera.isOpened()) return 1;

	Mat frame;

	while (true) {

		if (!camera.read(frame)) { break; }

		imshow("Video1", frame);

		int c = cvWaitKey(40);

		if(c == 27)
			break;
	}

	return 0;

}