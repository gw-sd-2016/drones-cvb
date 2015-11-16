#include <stdio.h>
#include <iostream>

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

int main(int argc, char** argv) {

	Mat image = imread(argv[1], IMREAD_GRAYSCALE);
	Mat scene = imread(argv[2], IMREAD_GRAYSCALE);

	if (!image.data || !scene.data) {
		cerr << "Unable to open provided images" << endl;
		exit(EXIT_FAILURE);
	}

	int minHessian = 500;

	// Detect keypoints using SURF Detector
	Ptr<FeatureDetector> detector = SurfFeatureDetector::create(minHessian);
	vector<KeyPoint> kp_image, kp_scene;

	detector->detect(image, kp_image);
	detector->detect(scene, kp_scene);

	// Calculate descriptors
	Ptr<SURF> extractor = SURF::create();
	Mat des_image, des_scene;

	extractor->compute(image, kp_image, des_image);
	extractor->compute(scene, kp_scene, des_scene);

	// Match descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(des_image, des_scene, matches);
	//matcher.match(des_image, des_scene, matches, 2);

	// Calculate min and max distances between keypoints
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
		scene_vector.push_back(kp_scene[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(image_vector, scene_vector, CV_RANSAC);

	vector<Point2f> img_corners(4);
	img_corners[0] = cvPoint(0, 0);
	img_corners[1] = cvPoint(image.cols, 0);
	img_corners[2] = cvPoint(image.cols, image.rows);
	img_corners[3] = cvPoint(0, image.rows);
	vector<Point2f> scene_corners(4);

	perspectiveTransform(img_corners, scene_corners, H);

	line(img_matches, scene_corners[0] + Point2f(image.cols, 0), scene_corners[1] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(image.cols, 0), scene_corners[2] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(image.cols, 0), scene_corners[3] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(image.cols, 0), scene_corners[0] + Point2f(image.cols, 0), Scalar(0, 255, 0), 4);

	imshow("SURF", img_matches);
	//resizeWindow("SURF", 800, 800);

	waitKey(0);

	return 0;
}
