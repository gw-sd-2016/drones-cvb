/*
	Christoph von Braun
	mainProc.cpp
*/

#include "to3D.cpp"
#include "liveSurf.cpp"
#include "processCoordinates.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

// hardcoded value of number of cameras to analyze
#define NUM_CAMERAS 4

/**
	This program starts the processing of the coordinates that were detected from text files.
	Expects that crats has been running successfully upon launch, and that all four camera
	coordinate files are present.
**/

int main(int argc, char** argv) {

	printf("mainProc.cpp started.\n");

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

	printf("mainProc.cpp ended.\n");

}