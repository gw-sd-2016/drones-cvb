#include "to3D.cpp"
#include "liveSurf.cpp"
#include "colorDetection.cpp"
#include "processCoordinates.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <iostream>

#define NUM_CAMERAS 4

int main(int argc, char** argv) {

	printf("Main program started.\n");

	// check for argv not null and contains integer
	int cameraIndex = 999;
	try {
		if (argv[1] != NULL)
			cameraIndex = std::stoi(argv[1]);
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
	cout << "Camera index is " << cameraIndex << ".\n";

	/*
		Runs SURF on inputted camera. Output text file will be filled.
		Process coordinates within text file
	*/

	//cleanCoordinates(numCameras);

	// for testing purposes, nice to be able to not run SURF if desired
	if (cameraIndex != 0) {

		pthread_t threads[NUM_CAMERAS];
		struct thread_data td[NUM_CAMERAS];
		int rc;

		for (int i = 1; i <= NUM_CAMERAS; i++) {
			cout << "Creating thread " << i << endl;
			td[i].cameraIndex = i;
			rc = pthread_create(&threads[i], NULL, detectColor, (void *)&td[i]);

			if (rc) {
				cout << "Error: unable to create thread," << rc << endl;
				exit(-1);
			}
		}

		//processCoordinates(numCameras-1);
	}

	/*
	vector<Point2f> cam0, cam1, cam2, cam3;
	cam0 = processCoordinates(0);
	cam1 = processCoordinates(1);
	cam2 = processCoordinates(2);
	cam3 = processCoordinates(3);

	to3D(cam0, cam1, cam2, cam3);
	*/

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

	pthread_exit(NULL);
	printf("Main program exited.\n");
	
}
