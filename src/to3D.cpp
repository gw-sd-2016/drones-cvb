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


/*
	Calculates the 3D point from two 2D points
*/
Point3f to3D(Point2f p1, Point2f p2, Point2f p3, Point2f p4) {
	
	// only going to assume that p1 and p2 are in use for now
	// CALCULATION HERE
	return Point3f(-1, -1, -1);
}
