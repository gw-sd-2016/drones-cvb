#include "liveSurf.cpp"
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

	// starting threads
	std::cout << "Spawning threads...\n";
	std::thread t0 (liveSurf, 0);
	std::thread t1 (liveSurf, 1);
	std::cout << "Done spawning threads.\n";
	std::cout << "Waiting for them to join...\n";
	t0.join();
	t1.join();
	std::cout << "All threads joined.\n"

	// from here, we know that numCameras is a valid int
	// this should never fail
	printf("Running liveSurf with %d cameras\n", numCameras);
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