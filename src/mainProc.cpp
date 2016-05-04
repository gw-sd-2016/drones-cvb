/*
	Christoph von Braun
	mainProc.cpp
*/

#include "processCoordinates.cpp"
#include <fcntl.h>
#include "opencv.h"
#include "numCameras.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

// define buffer size for piping
#define MAX_BUF 1024

void processCameras() {

	// hard coded output of camera calibration
	// currently only intrinsic matrix and distortion coefficients

	// hard coded camera values
	double FRONT_CAMERA_MATRIX[3][3] = {{1.4049474067879219e+03, 0, 6.3950000000000000e+02}, 
										{0, 1.4049474067879219e+03, 4.7950000000000000e+02}, 
										{0, 0, 1}};

	double FRONT_DISTORTION_COEFFICIENTS[5][1] = {{-3.7335300703975699e-01}, {1.7185670573063845e+01}, 
												  {0}, {0}, {-2.7437573692376083e+02}};

    double SIDE_CAMERA_MATRIX[3][3] = {{1.3626453437413036e+03, 0, 6.3950000000000000e+02},
    								   {0, 1.3626453437413036e+03, 4.7950000000000000e+02},
    								   {0, 0, 1}};

    double SIDE_DISTORTION_COEFFICIENTS[5][1] = {{-4.1374104405962936e-01}, {1.0376994656068659e+01},
    											 {0}, {0}, {-9.1610858982185292e+01}};

	Mat front_intrinsics, front_distCoeffs;
	Mat side_intrinsics, side_distCoeffs;
	Mat rvec, tvec, rotationMatrix;
	Mat rvec2, tvec2, rotationMatrix2;

	vector<Point3d> world_coords;
	vector<Point2d> pixel_coords;

	vector<Point3d> world_coords2;
	vector<Point2d> pixel_coords2;

	/*
		Values here are based on calibration of cameras
		with real object
	*/

	world_coords.push_back(Point3d(3.251, 1.6002, 1.8504));		// side cam, top right
	world_coords.push_back(Point3d(3.251, 1.3970, 1.8288));		// side cam, bottom right
	world_coords.push_back(Point3d(3.251, 1.3462, 2.1082));		// side cam, bottom left
	world_coords.push_back(Point3d(3.251, 1.5494, 2.1590));		// side cam, top left

	pixel_coords.push_back(Point2d(723, 380));					// side cam, top right
	pixel_coords.push_back(Point2d(744, 468));					// side cam, bottom right
	pixel_coords.push_back(Point2d(622, 485));					// side cam, bottom left
	pixel_coords.push_back(Point2d(606, 397));					// side cam, top left

	world_coords2.push_back(Point3d(1.2319, 1.9177, 2.0574));	// front cam, top right
	world_coords2.push_back(Point3d(1.4605, 1.7463, 1.8288));	// front cam, bottom right
	world_coords2.push_back(Point3d(0.7874, 1.7463, 1.8288));	// front cam, bottom left
	world_coords2.push_back(Point3d(0.7620, 1.9177, 1.8542));	// front cam, top left

	pixel_coords2.push_back(Point2d(760, 344));					// front cam, top right
	pixel_coords2.push_back(Point2d(985, 504));					// front cam, bottom right
	pixel_coords2.push_back(Point2d(423, 469));					// front cam, bottom left
	pixel_coords2.push_back(Point2d(364, 275));					// front cam, top left

	front_intrinsics = Mat(3, 3, DataType<double>::type, &FRONT_CAMERA_MATRIX);
	front_distCoeffs = Mat(5, 1, DataType<double>::type, &FRONT_DISTORTION_COEFFICIENTS);
	side_intrinsics = Mat(3, 3, DataType<double>::type, &SIDE_CAMERA_MATRIX);
	side_distCoeffs = Mat(5, 1, DataType<double>::type, &SIDE_DISTORTION_COEFFICIENTS);

	solvePnP(world_coords, pixel_coords, side_intrinsics, side_distCoeffs, rvec, tvec);
	solvePnP(world_coords2, pixel_coords2, front_intrinsics, front_distCoeffs, rvec2, tvec2);

	// side cam
	Mat cameraRotationVector, R;
	Rodrigues(rvec, R);
	Rodrigues(R.t(), cameraRotationVector);
	Mat cameraTranslationVector = -R.t() * tvec;


	// front cam
	Mat cameraRotationVector2, R2;
	Rodrigues(rvec2, R2);
	Rodrigues(R2.t(), cameraRotationVector2);
	Mat cameraTranslationVector2 = -R2.t() * tvec2;

	// Camera coordinates contained in cameraTranslationVector
	// Camera pose contained in cameraRotationVector

	// Printing them out
	cout << "Side Camera coordinates: " << endl << " " << cameraTranslationVector << endl << endl;
	cout << "Side Camera pose: " << endl << " " << cameraRotationVector << endl << endl;
	cout << "Front Camera coordinates: " << endl << " " << cameraTranslationVector2 << endl << endl;
	cout << "Front Camera pose: " << endl << " " << cameraRotationVector2 << endl << endl;
}
	

/**
	Delimiter function for applying to incoming data in pipe
**/
Point2f delimiter2(string str) {

	string delim = ",";
	size_t pos = 0;
	string token;

	pos = str.find(delim);

	string x = str.substr(0, pos);
	string y = str.substr(pos + delim.length(), str.length());

	return Point(stoi(x), stoi(y));
}

/**
	This program starts the processing of the coordinates that were detected from text files.
	Expects that crats has been running successfully upon launch, and that all four camera
	coordinate files are present.
**/

int main(int argc, char** argv) {

	printf("mainProc.cpp started.\n");

	// opening the first pipe for reading in from color detection
	int num, fifo;
	char temp[38];
	if ((fifo = open("/tmp/fifo", O_RDONLY)) < 0) {
		printf("%s\n", strerror(errno));
		return 0;
	}

	// opening the second pipe for writing out to display program
	int num2, fifo2;
	fifo2 = mkfifo("/tmp/fifo2", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	if ((fifo2 = open("/tmp/fifo2", O_WRONLY)) < 0) {
		printf("%s\n", strerror(errno));
		return 0;
	}

	int i = 1;
	string one, two, three, four;

	processCameras();

	// Zeroing out strings so those greater than NUM_CAMERAS doesn't crash.
	// Will be treated as if coordinate not found for that camera.
	one = "-1";
	two = "-1";
	three = "-1";
	four = "-1";

	while (true) {

		if ((num = read(fifo, temp, sizeof(temp))) < 0) {
			printf("%s\n", strerror(errno));
		}

		// converts temp (pipe contents) to string and sets it according to
		// its camera number
		switch (i) {
			case 1: one = string(temp);
					break;
			case 2: two = string(temp);
					break;
			case 3: three = string(temp);
					break;
			case 4: four = string(temp);
					break;
		}

		printf("%s from camera %d\n", temp, i);

		// loops from 1 to NUM_CAMERAS
		if (i != NUM_CAMERAS) { i++; }
		else {
			Point3f finalCoordinate = processCoordinates(one, two, three, four);
			ostringstream ssx, ssy, ssz;
			ssx << finalCoordinate.x;
			ssy << finalCoordinate.y;
			ssz << finalCoordinate.z;
			string sx(ssx.str());
			string sy(ssy.str());
			string sz(ssz.str());
			string finalCoordinateStr = sx + ", " + sy + ", " + sz;

			// test for proximity to hard-coded values of demo
			Point2f pt = delimiter2(one);
			string additionalStr = "\n";
			int threshold = 100;
			int hardX = 555;
			int hardY = 375;
			if ((abs(pt.x-100) < threshold) && (abs(pt.y-100) < threshold)) {
				additionalStr = "\n Match found!\n";
			}

			string outputStr = "2D coordinates: " + one + ". 3D coordinates: " + finalCoordinateStr + additionalStr;
			char* outputArray = (char*)outputStr.c_str();
			if ((num2 = write(fifo2, outputArray, strlen(outputArray) + 1)) < 0) {
				printf("ERROR: %s\n", strerror(errno));
			}

			if (finalCoordinate.x == -1 || finalCoordinate.y == -1 || finalCoordinate.z == -1) {
				printf("Coordinate processing failed\n");
			}

			i = 1;
		}
	}

	close(fifo);

	printf("mainProc.cpp ended.\n");

}

