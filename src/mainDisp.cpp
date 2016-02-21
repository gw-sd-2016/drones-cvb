#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "opencv.h"

#define WIDTH 1280
#define HEIGHT 960

using namespace std;
using namespace cv;

/**
	Delimiter function for applying to incoming data in pipe
**/
Point delimiter(string str) {

	string delim = ",";
	size_t pos = 0;
	string token;

	pos = str.find(delim);

	string x = str.substr(0, pos);
	string y = str.substr(pos + delim.length(), str.length());

	return Point(stoi(x), stoi(y));
}

/**
	Draws a fixed-size filled circle
**/
void drawCircle(Mat img, Point center) {

	int thickness = -1;
	int lineType = 8;
	int radius = 5;

	circle(img, center, radius, Scalar(0, 0, 255), thickness, lineType);

}

/**
	This program reads in coordinates from a pipe and displays them
	on a 2-D coordinate system
**/
int main(int argc, char** argv) {

	printf("Display program started.\n");

	int num, fifo;
	char temp[38];

	// opening the pipe
	if ((fifo = open("/tmp/fifo2", O_RDONLY)) < 0) {
		printf("%s\n", strerror(errno));
		return 0;
	}

	// creating the window
	char window[] = "Display Window";
	Mat display_image = Mat::zeros(WIDTH, HEIGHT, CV_8UC3);

	while (true) {

		if ((num = read(fifo, temp, sizeof(temp))) < 0) {
			printf("%s\n", strerror(errno));
		}

		printf("In FIFO2 is %s \n", temp);
		string str(temp);
		Point pt = delimiter(str);
		printf("Delimited to %d %d\n", pt.x, pt.y);
		drawCircle(display_image, pt);
		imshow(window, display_image);
		waitKey(2);

	}
}