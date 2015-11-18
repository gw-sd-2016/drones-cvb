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

	processCoordinates(numCameras-1);
	liveSurf(numCameras-1);

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
