/*
	Christoph von Braun
	mainProc.cpp
*/

#include "to3D.cpp"
#include "liveSurf.cpp"
#include "processCoordinates.cpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

// hardcoded value of number of cameras to analyze
#define NUM_CAMERAS 4

// define buffer size for piping
#define MAX_BUF 1024

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

	while (true) {

		if ((num = read(fifo, temp, sizeof(temp))) < 0) {
			printf("%s\n", strerror(errno));
		}
		printf("In FIFO is %s \n", temp);

		// converts temp (pipe contents) to string and sets it according to
		// its camera number
		switch (i) {
			case 1: one = string(temp);
			case 2: two = string(temp);
			case 3: three = string(temp);
			case 4: four = string(temp);
		}

		printf("Saved to switch %d\n", i);

		// loops 1-2-3-4-1-2-3-4
		if (i != 4) { i++; }
		else {
			
			printf("Sending data to processor...\n");

			int ret = processCoordinates(one, two, three, four);

			if (ret == 1) {
				printf("Coordinate processing failed\n");
				return 0;
			}

			else { printf("Coordinate processed succeeded. Continuing...\n"); }

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