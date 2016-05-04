/*
	Christoph von Braun
	colorDetection.cpp
*/

#include "opencv.h"
#include "numCameras.h"
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

// hardcoded value of number of cameras to analyze
//#define NUM_CAMERAS 4

int kb;

/**
	Takes in a Matrix as a frame of a camera, and processes it for all instances of red.
	Records the results in a text file with the coordinates of the instance of red in the frame
**/
Point calculateColor(Mat bgr_image) {

	Mat hsv_image;

	// filter out some noise from the image
	medianBlur(bgr_image, bgr_image, 3);

	// convert BGR to HSV and add red range
	cvtColor(bgr_image, hsv_image, COLOR_BGR2HSV);		
	Mat lower_red_hue_range;
	Mat upper_red_hue_range;
	inRange(hsv_image, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
	inRange(hsv_image, Scalar(160, 100, 100), Scalar(179, 255, 255), upper_red_hue_range);

	// combine the above two images
	Mat red_hue_image;
	addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

	// add some blur for noise
	GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);

	vector<Vec3f> circles;
	HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows/8, 100, 20, 0, 0);


	// loop over all detected circles and return the center of the first one.
	// Important assumption here: there is only one red area found in the image.
	// All subsequent red areas will be disregarded. Might implement maximum radius 
	// calculation in the future.
	for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
		Point center(round(circles[current_circle][0]), round(circles[current_circle][1]));
		int radius = round(circles[current_circle][2]);

		// write coordinates to file
		circle(red_hue_image, center, radius, Scalar(0, 255, 0), 5);
		return center;
	}

	// if no circles have been detected, write (-1, -1) to signify
	if (circles.size() == 0) {
		return Point(-1, -1);
	}

	// doesn't get here. just to shut compiler up.
	return Point(-1, -1);
}

/**
	Sets up the cameras and output files, then runs color detection
	on each frame of each camera in succession
**/
void detectColor() {

	VideoCapture capture[NUM_CAMERAS];
	Mat camFrames[NUM_CAMERAS];
	string labels[NUM_CAMERAS];
	ofstream outputFiles[NUM_CAMERAS];
	Mat processedFrames[NUM_CAMERAS];

	string strPhrase;
	char * phrase;

	// opens the pipe
	// this serves as the writing portion of the pipe
	int num, fifo;
	if ((fifo = open("/tmp/fifo", O_WRONLY)) <  0) {
		printf("%s\n", strerror(errno));
		return;
	}

	// setup loop; opens the camera streams and output files
	int j;
	for (int i = 1; i <= NUM_CAMERAS; i++) {
		// open the cameras
		if (i == 1) {
			j = CAM_ONE;
		}
		else if (i == 2) {
			j = CAM_TWO;
		}

		labels[i-1] = "Camera " + to_string(j);
		capture[i-1].open(j);

		// open the output files
		cout << "Opening camera" << to_string(j) << ".txt\n";
		outputFiles[i-1].open("camera" + to_string(j) + ".txt");
	}

	printf("Setup complete.\n");

	// setup window for displaying
	char feed1[] = "Camera Feed 1";
	char feed2[] = "Camera Feed 2";


	// loops forever; don't want a sleep() here because calculateColor is slow enough
	// I'm already missing some frames here.
	while (true) {

		// runs calculateColor() on each frame of each camera
		for (int i = 1; i <= NUM_CAMERAS; i++) {
			capture[i-1] >> camFrames[i-1];
			Point p = calculateColor(camFrames[i-1]);
			outputFiles[i-1] << p.x << "," << p.y << endl;				// endl also flushes the line out
			cout << p.x << ", " << p.y << " written from camera " << to_string(i) << "\n";

			// display frame and detected location
			circle(camFrames[i-1], p, 5, Scalar(0, 255, 0), -1, 8);
			if (i == 1) {
				imshow(feed1, camFrames[i-1]);
			}
			else if (i == 2) {
				imshow(feed2, camFrames[i-1]);
			}
			
			// string to char* conversion code
			strPhrase = to_string(p.x) + "," + to_string(p.y);
			phrase = new char[strPhrase.size() + 1];
			copy(strPhrase.begin(), strPhrase.end(), phrase);
			phrase[strPhrase.size()] = '\0';


			if ((num = write(fifo, phrase, strlen(phrase) + 1)) < 0) {
				printf("ERROR: %s\n", strerror(errno));
			}

			waitKey(1);				// forces the windows to display
			delete[] phrase;
		}
	}

	// closing the output files
	for (int i = 1; i <= NUM_CAMERAS; i++) {
		outputFiles[i-1].close();
	}

	// closing the pipe
	close(fifo);
} 

