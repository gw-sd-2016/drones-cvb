#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

void homogenous(Point2f p) {

	/*
		STEPS
		1. Undistort 2D coordinate with distortion coefficients
		2. Convert to ray
		3. Move the ray into proper coordinate system
	*/

    double SIDE_CAMERA_MATRIX[3][3] = {{1.3626453437413036e+03, 0, 6.3950000000000000e+02},
    								   {0, 1.3626453437413036e+03, 4.7950000000000000e+02},
    								   {0, 0, 1}};

	double point_array[3][1] = {p.x, p.y, 1};

	Mat side_intrinsics = Mat(3, 3, DataType<float>::type, &SIDE_CAMERA_MATRIX);
	Mat hom_pt = Mat(3, 1, DataType<float>::type, &point_array);

	// multiply homogenous point by inverse of intrinsics matrix
	hom_pt = side_intrinsics.inv() * hom_pt;

	Point3f origin(0, 0, 0);
	Point3f direction(hom_pt.at<float>(0, 0), hom_pt.at<float>(1, 0), hom_pt.at<float>(2, 0));

	// to get a unit vector, the direction needs to be normalized
	direction *= 1/norm(direction);

	cout << "Homogenous point: " << endl << " " << hom_pt << endl << endl;
	printf("Normalized direction vector: (%f, %f, %f)\n", direction.x, direction.y, direction.z);

}

void to3D(Point2f p1, Point2f p2, Point2f p3, Point2f p4) {

	// generate output file of 3D coordinates
	ofstream outputFile;
	outputFile.open("3D.txt");

	// all possible quadrant combinations

	// no 0 but 1 2 3; three quadrants possible
	if (p1.x == -1 && p2.x != -1 && p3.x != -1 && p4.x != -1) {
		outputFile << "all but northwest\n";
	}

	// no 1 but 0 2 3; three quadrants possible
	if (p1.x != -1 && p2.x == -1 && p3.x != -1 && p4.x != -1) {
		outputFile << "all but northeast\n";
	}

	// no 2 but 0 1 3; three quadrants possible
	if (p1.x != -1 && p2.x != -1 && p3.x == -1 && p4.x != -1) {
		outputFile << "all but southeast\n";
	}

	// no 3 but 0 1 2; three quadrants possible
	if (p1.x != -1 && p2.x != -1 && p3.x != -1 && p4.x == -1) {
		outputFile << "all but southwest\n";
	}

	// all 1 of 4 accounted for
	// now 2 of 2

	// 0 1 but no 2 3; half possible
	if (p1.x != -1 && p2.x != -1 && p3.x == -1 && p4.x == -1) {
		outputFile << "north half\n";
	}

	// 1 2 but no 0 3; half possible
	if (p1.x == -1 && p2.x != -1 && p3.x != -1 && p4.x == -1) {
		outputFile << "east half\n";
	}

	// 2 3 but no 0 1; half possible
	if (p1.x == -1 && p2.x == -1 && p3.x != -1 && p4.x != -1) {
		outputFile << "south half\n";
	}

	// 0 3 but no 1 2; half possible
	if (p1.x != -1 && p2.x == -1 && p3.x == -1 && p4.x != -1) {
		outputFile << "west half\n";
	}

	// 0 2 but no 1 3; unknown outcome
	if (p1.x != -1 && p2.x == -1 && p3.x != -1 && p4.x == -1) {
		outputFile << "unknown location\n";
	}

	// 1 3 but no 0 2; unknown outcome
	if (p1.x == -1 && p2.x != -1 && p3.x == -1 && p4.x != -1) {
		outputFile << "unknown location\n";
	}

	// all 2 of 4 accounted for
	// now 1 of 4 known

	// 0 but no 1 2 3; one possible
	if (p1.x != -1 && p2.x == -1 && p3.x == -1 && p4.x == -1) {
		outputFile << "northwest corner\n";
	}

	// 1 but no 0 2 3; one possible
	if (p1.x == -1 && p2.x != -1 && p3.x == -1 && p4.x == -1) {
		outputFile << "northeast corner\n";
	}

	// 2 but no 0 1 3; one possible
	if (p1.x == -1 && p2.x == -1 && p3.x != -1 && p4.x == -1) {
		outputFile << "southeast corner\n";
	}

	// 3 but no 0 1 2; one possible
	if (p1.x == -1 && p2.x == -1 && p3.x == -1 && p4.x != -1) {
		outputFile << "southwest corner\n";
	}

	// all 1 of 4 known accounted for
	// now all or nothing

	// no knowns
	if (p1.x == -1 && p2.x == -1 && p3.x == -1 && p4.x == -1) {
		outputFile << "no drone found\n";
	}

	// all knowns; this is the hard part
	if (p1.x != -1 && p2.x != -1 && p3.x != -1 && p4.x != -1) {
		outputFile << "all cams found\n";

		// camera viewport is 1280 * 960

		/*
		Might be able to calculate height separately. Use next wall's camera to find
		object distance to the first camera from its x-coordinate.
		If close to camera, high drone = high in viewport
		If far from camera, high drone = maybe 3/4 up. Not possible to hit highest point with distance
		*/
		
	}

	outputFile.close();

}