/*
	Christoph von Braun
	main.cpp
*/

#include "colorDetection.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

/**
	This program starts the color detection, which is
	expected to run until user quits it.
**/
int main(int argc, char** argv) {

	printf("Color detection started.\n");

	/*
		Runs color detection on all cameras and saves coordinates
		as a text file under camerax.txt
	*/

	if (NUM_CAMERAS != 0) {	detectColor(); }

	printf("Color detection exited.\n");
	
}
