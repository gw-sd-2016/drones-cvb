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

/* UNDER CONSTRUCTION 
void processCameras() {

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

	front_intrinsics = Mat(3, 3, DataType<double>::type, &FRONT_CAMERA_MATRIX);
	front_distCoeffs = Mat(5, 1, DataType<double>::type, &FRONT_DISTORTION_COEFFICIENTS);
	side_intrinsics = Mat(3, 3, DataType<double>::type, &SIDE_CAMERA_MATRIX);
	side_distCoeffs = Mat(5, 1, DataType<double>::type, &SIDE_DISTORTION_COEFFICIENTS);

	rvec.create(1, 3, DataType<double>::type);
	tvec.create(1, 3, DataType<double>::type);
	rotationMatrix.create(3, 3, DataType<double>::type);

	solvePnP(worldPlane, imagePlane, front_intrinsics, front_distCoeffs, rvec, tvec);
	Rodrigues(rvec, rotationMatrix);

}
*/	

/**
	This program starts the processing of the coordinates that were detected from text files.
	Expects that crats has been running successfully upon launch, and that all four camera
	coordinate files are present.
**/

int main(int argc, char** argv) {

	printf("mainProc.cpp started.\n");

	int num, fifo;
	char temp[38];
	if ((fifo = open("/tmp/fifo", O_RDONLY)) < 0) {
		printf("%s\n", strerror(errno));
		return 0;
	}

	int i = 1;
	string one, two, three, four;

	//processCameras();

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
		printf("In FIFO is %s \n", temp);

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

		printf("Saved to switch %d\n", i);

		// loops from 1 to NUM_CAMERAS
		if (i != NUM_CAMERAS) { i++; }
		else {
			
			printf("Sending data to processor...\n\n");

			int ret = processCoordinates(one, two, three, four);

			if (ret == 1) {
				printf("Coordinate processing failed\n");
				return 0;
			}

			else { printf("\nCoordinate processed succeeded. Continuing...\n"); }

			i = 1;
		}
	}

	close(fifo);


	/** DEPRECIATED CODE **/
	/*
	// opening text files
	ifstream cam1, cam2, cam3, cam4;
	string line1, line2, line3, line4;
	cam1.open("camera1.txt");
	cam2.open("camera2.txt");
	cam3.open("camera3.txt");
	cam4.open("camera4.txt");

	// this code block still under construction

	// Needs to read files while they are still being written to.
	// this will eventually call something in processCoordinates.cpp and to3D.cpp
	// for processing, cleaning up and combining the camera data
	if (cam1.is_open() && cam2.is_open() && cam3.is_open() && cam4.is_open()) {

		printf("Everything is open.\n");

		while (true) {
			while (getline(cam1, line1)) { cout << "Cam1: " << line1 << "\n"; }
			while (getline(cam2, line2)) { cout << "Cam2: " << line2 << "\n"; }
			while (getline(cam3, line3)) { cout << "Cam3: " << line3 << "\n"; }
			while (getline(cam4, line4)) { cout << "Cam4: " << line4 << "\n"; }

			if (!cam1.eof()) break;
			if (!cam2.eof()) break;
			if (!cam3.eof()) break;
			if (!cam4.eof()) break;

			cam1.clear();
			cam2.clear();
			cam3.clear();
			cam4.clear();

			sleep(5);		// prevents being a CPU hog

			cout << "One round\n";
		}

	}
	*/

	printf("mainProc.cpp ended.\n");

}

