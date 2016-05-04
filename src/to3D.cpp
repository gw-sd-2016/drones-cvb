#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "opencv2/core/core.hpp"
#include "opencv.h"

using namespace std;
using namespace cv;


Point3f homogenous(Point2f p, int camera) {

	/*
		STEPS
		1. Undistort 2D coordinate with distortion coefficients
		2. Convert to ray
		3. Move the ray into proper coordinate system
	*/

    double SIDE_CAMERA_MATRIX[3][3] = {{1.3626453437413036e+03, 0, 6.3950000000000000e+02},
    								   {0, 1.3626453437413036e+03, 4.7950000000000000e+02},
    								   {0, 0, 1}};

   	double FRONT_CAMERA_MATRIX[3][3] = {{1.4049474067879219e+03, 0, 6.3950000000000000e+02}, 
										{0, 1.4049474067879219e+03, 4.7950000000000000e+02}, 
										{0, 0, 1}};

	double FRONT_DISTORTION_COEFFICIENTS[5][1] = {{-3.7335300703975699e-01}, {1.7185670573063845e+01}, 
												  {0}, {0}, {-2.7437573692376083e+02}};

    double SIDE_DISTORTION_COEFFICIENTS[5][1] = {{-4.1374104405962936e-01}, {1.0376994656068659e+01},
    											 {0}, {0}, {-9.1610858982185292e+01}};									

	Mat side_intrinsics = Mat(3, 3, DataType<double>::type, &SIDE_CAMERA_MATRIX);
	Mat front_intrinsics = Mat(3, 3, DataType<double>::type, &FRONT_CAMERA_MATRIX);
    Mat side_distort = Mat(5, 1, DataType<double>::type, &SIDE_DISTORTION_COEFFICIENTS);
	Mat front_distort = Mat(5, 1, DataType<double>::type, &FRONT_DISTORTION_COEFFICIENTS);	

	/*
	// undistort point
	Point2f pUndistorted;
	float pArray[2][1] = {p.x, p.y};
	float pArrayUndistort[2][1];
	if (camera == 0) {
		undistortPoints(pArray, pArrayUndistort, front_intrinsics, front_distort);
	}
	else if (camera == 1) {
		undistortPoints(pArray, pArrayUndistort, side_intrinsics, side_distort);
	}

	cout << "  Distorted point: " << p.x << " " << p.y << endl;
	cout << "Undistorted point: " << pUndistorted.x << " " << pUndistorted.y << endl;
	*/

	double point_array[3][1] = {p.x, p.y, 1};
	Mat hom_mt = Mat(3, 1, DataType<double>::type, &point_array);

	// HERE GOOD \\

	// multiply homogenous point by inverse of intrinsics matrix
	if (camera == 0) {
		hom_mt = front_intrinsics.inv() * hom_mt;
		//printf("Front camera ");
	}
	else if (camera == 1) {
		hom_mt = side_intrinsics.inv() * hom_mt;
		//printf("Side camera ");
	}

	Point3f origin(0, 0, 0);
	Point3f hom_pt(hom_mt.at<double>(0, 0), hom_mt.at<double>(1, 0), hom_mt.at<double>(2, 0));
	Point3f direction(hom_pt.x, hom_pt.y, hom_pt.z);

	// to get a unit vector, the direction needs to be normalized
	direction *= 1/norm(direction);

	//printf("normalized direction vector: (%f, %f, %f)\n", direction.x, direction.y, direction.z);

	return direction;

}

/*
	Up next:

	Test out direction vectors
	Do I need to worry about distortion?
	Moving the ray into a coordinate system based on the camera pose
*/

/*
	Calculates the 3D point from two 2D points
*/
Point3f to3D(Point2f p1, Point2f p2, Point2f p3, Point2f p4) {

	// absolutely coordinate values of both cameras
	// values are in meters
	double FRONT_X = 1.143;
	double FRONT_Y = 1.7272;
	double FRONT_Z = 0;

	double SIDE_X = 0;
	double SIDE_Y = 1.7272;
	double SIDE_Z = 1.9304;

	// only going to assume that p1 and p2 are in use for now

	Point3f old_side = homogenous(p1, 1);
	Point3f old_front = homogenous(p2, 0);

	// Moves the direction vectors into the base coordinate system
	Point3f new_side, new_front;

	new_front.y = old_front.y + FRONT_Y;	// y coordinates remain the same for the conversion
	new_side.y = old_side.y + SIDE_Y;

	new_front.x = old_front.x + FRONT_X;
	new_side.x = old_side.z + SIDE_X; 		// JUNK (SIDE_X == 0)

	new_front.z = old_front.z + FRONT_Z;	// JUNK (FRONT_Z == 0)
	new_side.z = old_side.x + SIDE_Z;

	Point3f frontPt = new_front;
	Point3f sidePt = new_side;

	// Both front and side cameras are now in the proper X Y Z system (probably)

	// Need to create lines from camera coordiate to frontPt and sidePt
	// Front and side Y should be equal
	// Front Z is junk (use side z instead)
	// Side X is junk (use front x instead)

	// FINAL COORDINATE
	// X = frontPt.x;
	// Y = (frontPt.y + sidePt.y)/2;
	// Z = sidePt.z;

	Point3f finalCoordinate = Point3f(frontPt.x, (frontPt.y + sidePt.y)/2, sidePt.z);

	// converting to feet
	printf("Final Coordinate: %f %f %f\n", finalCoordinate.x * 3.2808, finalCoordinate.y * 3.2808, finalCoordinate.z * 3.2808);

	return finalCoordinate;
}
