	/*
	Christoph von Braun
	main.cpp
*/

#include "colorDetection.cpp"
#include "numCameras.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
	This program starts the color detection, which is
	expected to run until user quits it.
**/
int main(int argc, char** argv) {

	printf("Color detection started.\n");

	// establishes the named pipe
	int fifo = mkfifo("/tmp/fifo", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);

	/*
		Runs color detection on all cameras and writes coordinates into named pipe
	*/

	if (NUM_CAMERAS != 0) {	detectColor(); }

	printf("Color detection exited.\n");
	
}
