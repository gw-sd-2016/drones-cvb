/*
	Christoph von Braun
	colorDetection.cpp
*/

#include "threadData.h"
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <fstream>

#include "opencv2/core/core.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/opencv_modules.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

int kb;

#define NUM_CAMERAS 4

void calculateColor(Mat bgr_image) {

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


	// loop over all detected circles and outline them on the original image
	for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
		Point center(round(circles[current_circle][0]), round(circles[current_circle][1]));
		int radius = round(circles[current_circle][2]);

		circle(red_hue_image, center, radius, Scalar(0, 255, 0), 5);
	}

	// show the result
	imshow("Color Detection", red_hue_image);
	
}

void detectColor() {

	// initializing video captures
	VideoCapture capture[NUM_CAMERAS];
	Mat camFrames[NUM_CAMERAS];
	string labels[NUM_CAMERAS];

	for (int i = 1; i <= NUM_CAMERAS; i++) {
		labels[i] = "Camera " + to_string(i);
		capture[i].open(i);
	}

	// loop until quit command
	while ((char)kb != 'q') {

		for (int i = 1; i <= NUM_CAMERAS; i++) {
			capture[i] >> camFrames[i];
			calculateColor(camFrames[i]);
		}
	}
} 

