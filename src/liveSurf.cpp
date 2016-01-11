/*
	Christoph von Braun
	liveSurf.cpp
*/

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

int keyboard;

/*
	Calculates if four given points resemble rectangle.
	Not backwards, severerly elongated, etc...
*/
bool isRectangleish(Point2f a, Point2f b, Point2f c, Point2f d) {
	
	return(a.x < b.x && a.x < c.x && a.y < c.y && a.y < d.y);
}

/*
	Calculates the average center coordinate given four points
*/
Point2f calculateCenter(Point2f a, Point2f b, Point2f c, Point2f d) {
	int x = (a.x + b.x + c.x + d.x)/4;
	int y = (a.y + b.y + c.y + d.y)/4;

	return Point2f(x, y);
}

/*
	Needs to return coordinates of drone
	Try writing to a text file...
*/
int liveSurf(int camera) {

	// make output file based on the number of the camera inputted
	ofstream outputFile;
	outputFile.open("camera" + to_string(camera) + ".txt");

	//Mat origImage = imread(argv[1], IMREAD_GRAYSCALE);
	Mat origImage = imread("pattern.jpg", IMREAD_GRAYSCALE);
	Mat image;
	Mat scene;
	bool previousMatch = false;
	bool found = true;

	transpose(origImage, image);
	flip(image, image, 1);

	// Open and check video streams; check image
	VideoCapture capture(camera);
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

	// compute SURF on images
	detector->detect(image, kp_image);
	Mat des_image, des_scene;
	extractor->compute(image, kp_image, des_image);

	// loop until quit command
	while ((char)keyboard != 'q') {

		// make sure camera is readable
		if (!capture.read(scene)) {
			cerr << "Unable to read next frame" << endl;
			exit(EXIT_FAILURE);
		}

		// compute SURF on frame
		detector->detect(scene, kp_scene);
		extractor->compute(scene, kp_scene, des_scene);

		if (!des_scene.empty()) {
			// matcher on frame
			matcher.match(des_image, des_scene, matches);

			// calculate distances
			double max_dist = 0; double min_dist = 100;

			for (int i = 0; i < des_image.rows; i++) {
				double dist = matches[i].distance;
				if (dist < min_dist) min_dist = dist;
				if (dist > max_dist) max_dist = dist;
			}

			// determine good matches
			vector<DMatch> good_matches;

			for (int i = 0; i < des_image.rows; i++) {
				if (matches[i].distance < 3*min_dist) {
					good_matches.push_back(matches[i]);
				}
			}

			// some magic
			Mat img_matches;
			drawMatches(image, kp_image, scene, kp_scene, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

			vector<Point2f> image_vector;
			vector<Point2f> scene_vector;

			for (int i = 0; i < good_matches.size(); i++) {
				image_vector.push_back(kp_image[good_matches[i].queryIdx].pt);	
				scene_vector.push_back(kp_scene[good_matches[i].trainIdx].pt);
			}

			// some more magic
			Mat H = findHomography(image_vector, scene_vector, CV_RANSAC);
			
			int hRows = H.rows;
			int HCols = H.cols;
			Size s = H.size();

			vector<Point2f> img_corners(4);

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

			// drawing out the green box
			line(img_matches, scene_corners[0] + Point2f(image.cols, 0), scene_corners[1] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);
			line(img_matches, scene_corners[1] + Point2f(image.cols, 0), scene_corners[2] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);
			line(img_matches, scene_corners[2] + Point2f(image.cols, 0), scene_corners[3] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);
			line(img_matches, scene_corners[3] + Point2f(image.cols, 0), scene_corners[0] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);

			// Points a - d are the corners of the box
			Point2f a = scene_corners[0] + Point2f(image.cols, 0);
			Point2f b = scene_corners[1] + Point2f(image.cols, 0);
			Point2f c = scene_corners[2] + Point2f(image.cols, 0);
			Point2f d = scene_corners[3] + Point2f(image.cols, 0);

			// check that points mildly resemble a box
			if (isRectangleish(a, b, c, d)) {
				// draw a green point at the center of coodinates
				Point2f center = calculateCenter(a, b, c, d);
				circle(img_matches, center, 10, Scalar(0, 255, 0), -1);
				// write coordinates out to file; these are assumed to be found coordinates
				string out = to_string(center.x) + "," + to_string(center.y) + "\n";
				cout << "Writing to " << out;
				outputFile << out;
			}
			// otherwise write -1 to file for no coordinates found
			else {
				string out = to_string(-1) + "," + to_string(-1) + "\n";
				outputFile << out;
			}

			imshow("SURF", img_matches);
			keyboard = waitKey(25);
		}

	}
	
	outputFile.close();
	return 0;
} 
