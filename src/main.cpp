#include "to3D.cpp"
#include "liveSurf.cpp"
#include "processCoordinates.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>

int main(int argc, char** argv) {

	printf("Main program started.\n");

	// check for argv not null and contains integer
	int numCameras = 999;
	try {
		if (argv[1] != NULL)
			numCameras = std::stoi(argv[1]);
	} catch(std::exception const & e) {
		cout << "Error: " << e.what() << endl;
	}

	// prompt user for camera positions
	int i, j;
	cout << "Prototype for camera positions\n";
	cout << "This assumes four cameras in a rectangle\n";
	cout << "Please enter the length of the positions: ";
	cin >> i;
	cout << "Please enter the width of the positions: ";
	cin >> j;
	cout << "Camera coordinates are " << i << " and " << j << ".\n";


	/*
		Runs SURF on inputted camera. Output text file will be filled.
		Process coordinates within text file
	*/

	//cleanCoordinates(numCameras);

	// for testing purposes, nice to be able to not run SURF if desired
	if (numCameras != 0) {
		//liveSurf(numCameras-1);
		//wwprocessCoordinates(numCameras-1);
	}

	vector<Point2f> cam0, cam1, cam2, cam3;
	cam0 = processCoordinates(0);
	cam1 = processCoordinates(1);
	cam2 = processCoordinates(2);
	cam3 = processCoordinates(3);

	to3D(cam0, cam1, cam2, cam3);


	/*
	// if there are x cameras, the range of ints should be from [0, x]
	if (numCameras != 999) {
		// from 0 to numCameras - 1 inclusive
		for (int i = 0; i < numCameras; i++) {
			// need to do something with the output here
			liveSurf(i);
		}
	}
	*/

	printf("Main program exited.\n");
	
}
