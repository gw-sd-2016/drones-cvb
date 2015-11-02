// adding-multiple-images

#include "functions.h"
#include <stdio.h>
#include <iostream>
#include <cmath>

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

int keyboard;

/*
bool isRectangle(Point2f a, Point2f b, Point2f c, Point2f d) {

	// find the center of mass of corner points
	// test if square of distances from center mass to all corners are equal
	float cx, cy;
	float dd1, dd2, dd3, dd4;
	
	cx = (a.x + b.x + c.x + d.x)/4;
	cy = (a.y + b.y + c.y + d.y)/4;

	dd1 = sqrt(cx - a.x) + sqrt(cy - a.y);
	dd2 = sqrt(cx - b.x) + sqrt(cy - b.y);
	dd3 = sqrt(cx - c.x) + sqrt(cy - c.y);
	dd4 = sqrt(cx - d.x) + sqrt(cy - d.y);

	return(dd1 == dd2 && dd1 == dd3 && dd1 == dd4);
}

bool isRectangleish(Point2f a, Point2f b, Point2f c, Point2f d) {
	
	return(a.x < b.x && a.x < c.x && a.y < c.y && a.y < d.y);
}
*/

int liveSurf() {

	//Mat origImage = imread(argv[1], IMREAD_GRAYSCALE);
	Mat origImage = imread("drone-top.jpg", IMREAD_GRAYSCALE);
	Mat image;
	Mat scene;
	bool previousMatch = false;

	transpose(origImage, image);
	flip(image, image, 1);

	// Open and check video streams; check image
	VideoCapture capture(0);
	if(!capture.isOpened()) {
		cerr << "Unable to open video stream" << endl;
		exit(EXIT_FAILURE);
	}

	if (!image.data) {
		cerr << "Unable to open provided images" << endl;
		exit(EXIT_FAILURE);
	}

	int minHessian = 800;

	// Init SURF & matcher
	Ptr<FeatureDetector> detector = SurfFeatureDetector::create(minHessian);
	Ptr<SURF> extractor = SURF::create();
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	vector<KeyPoint> kp_image, kp_scene;

	// computer SURF on images
	detector->detect(image, kp_image);
	Mat des_image, des_scene;
	extractor->compute(image, kp_image, des_image);

	while ((char)keyboard != 'q') {

		if (!capture.read(scene)) {
			cerr << "Unable to read next frame" << endl;
			exit(EXIT_FAILURE);
		}

		detector->detect(scene, kp_scene);
		extractor->compute(scene, kp_scene, des_scene);

		if (!des_scene.empty()) {
			matcher.match(des_image, des_scene, matches);

			double max_dist = 0; double min_dist = 100;

			for (int i = 0; i < des_image.rows; i++) {
				double dist = matches[i].distance;
				if (dist < min_dist) min_dist = dist;
				if (dist > max_dist) max_dist = dist;
			}

			vector<DMatch> good_matches;

			for (int i = 0; i < des_image.rows; i++) {
				if (matches[i].distance < 3*min_dist) {
					good_matches.push_back(matches[i]);
				}
			}

			Mat img_matches;
			drawMatches(image, kp_image, scene, kp_scene, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

			vector<Point2f> image_vector;
			vector<Point2f> scene_vector;

			for (int i = 0; i < good_matches.size(); i++) {
				image_vector.push_back(kp_image[good_matches[i].queryIdx].pt);	
				//image_vector.push_back(kp_image[good_matches[i].trainIdx].pt);
				//scene_vector.push_back(kp_scene[good_matches[i].queryIdx].pt);
				scene_vector.push_back(kp_scene[good_matches[i].trainIdx].pt);
			}

			Mat H = findHomography(image_vector, scene_vector, CV_RANSAC);

			int hRows = H.rows;
			int HCols = H.cols;
			Size s = H.size();
			printf("Rows: %d, Cols: %d\n", s.height, s.width);

			/*
			0, 0
			cols, 0
			cols, rows
			0, rows
			*/

			vector<Point2f> img_corners(4);
			/*
			img_corners[0] = cvPoint((image.cols/3), (image.rows/3));
			img_corners[1] = cvPoint((2*image.cols/3), (image.rows/3));
			img_corners[2] = cvPoint((2*image.cols/3), (2*image.rows/3));
			img_corners[3] = cvPoint((image.cols/3), (2*image.rows/3));
			*/
			img_corners[0] = cvPoint(0, 0);
			img_corners[1] = cvPoint(image.cols, 0);
			img_corners[2] = cvPoint(image.cols, image.rows);
			img_corners[3] = cvPoint(0, image.rows);

			vector<Point2f> scene_corners(4);

			// perspectiveTransform needs H to be 3x3 matrix.
			// Moving the camera quickly sometimes gave 0x0
			if (s.height == 3 && s.width == 3) {
				perspectiveTransform(img_corners, scene_corners, H);
			}

			line(img_matches, scene_corners[0] + Point2f(image.cols, 0), scene_corners[1] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);
			line(img_matches, scene_corners[1] + Point2f(image.cols, 0), scene_corners[2] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);
			line(img_matches, scene_corners[2] + Point2f(image.cols, 0), scene_corners[3] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);
			line(img_matches, scene_corners[3] + Point2f(image.cols, 0), scene_corners[0] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);

			Point2f a = scene_corners[0] + Point2f(image.cols, 0);
			Point2f b = scene_corners[1] + Point2f(image.cols, 0);
			Point2f c = scene_corners[2] + Point2f(image.cols, 0);
			Point2f d = scene_corners[3] + Point2f(image.cols, 0);

			/*
			if (isRectangle(a, b, c, d) && previousMatch) {
				printf("Match, baby!");
				numMatches++;
			}
			else if (isRectangle(a, b, c, d)) {
				previousMatch = true;
			}
			else {
				previousMatch = false;
			}
			*/

			//if (isRectangleish(a, b, c, d)) { printf("\nMatch, ish\n\n"); }			

			imshow("SURF", img_matches);
			keyboard = waitKey(25);
		}

	}
	
	printf("%d total matches\n", numMatches);

	return 0;
} 
