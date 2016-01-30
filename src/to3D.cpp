#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

void to3D(Point2f p1, Point2f p2, Point2f p3, Point2f p4) {

	// generate output file of 3D coordinates
	ofstream outputFile;
	outputFile.open("3D.txt");

	// all possible quadrant combinations

	// no 0 but 1 2 3; three quadrants possible
	if (p1.x == -1 && p2.x != -1 && p3.x != -1 && p4.x != -1) {
		outputFile << "all but northwest\n";
	}

	// no 1 but 0 2 3; three quadrants possible
	if (p1.x != -1 && p2.x == -1 && p3.x != -1 && p4.x != -1) {
		outputFile << "all but northeast\n";
	}

	// no 2 but 0 1 3; three quadrants possible
	if (p1.x != -1 && p2.x != -1 && p3.x == -1 && p4.x != -1) {
		outputFile << "all but southeast\n";
	}

	// no 3 but 0 1 2; three quadrants possible
	if (p1.x != -1 && p2.x != -1 && p3.x != -1 && p4.x == -1) {
		outputFile << "all but southwest\n";
	}

	// all 1 of 4 accounted for
	// now 2 of 2

	// 0 1 but no 2 3; half possible
	if (p1.x != -1 && p2.x != -1 && p3.x == -1 && p4.x == -1) {
		outputFile << "north half\n";
	}

	// 1 2 but no 0 3; half possible
	if (p1.x == -1 && p2.x != -1 && p3.x != -1 && p4.x == -1) {
		outputFile << "east half\n";
	}

	// 2 3 but no 0 1; half possible
	if (p1.x == -1 && p2.x == -1 && p3.x != -1 && p4.x != -1) {
		outputFile << "south half\n";
	}

	// 0 3 but no 1 2; half possible
	if (p1.x != -1 && p2.x == -1 && p3.x == -1 && p4.x != -1) {
		outputFile << "west half\n";
	}

	// 0 2 but no 1 3; unknown outcome
	if (p1.x != -1 && p2.x == -1 && p3.x != -1 && p4.x == -1) {
		outputFile << "unknown location\n";
	}

	// 1 3 but no 0 2; unknown outcome
	if (p1.x == -1 && p2.x != -1 && p3.x == -1 && p4.x != -1) {
		outputFile << "unknown location\n";
	}

	// all 2 of 4 accounted for
	// now 1 of 4 known

	// 0 but no 1 2 3; one possible
	if (p1.x != -1 && p2.x == -1 && p3.x == -1 && p4.x == -1) {
		outputFile << "northwest corner\n";
	}

	// 1 but no 0 2 3; one possible
	if (p1.x == -1 && p2.x != -1 && p3.x == -1 && p4.x == -1) {
		outputFile << "northeast corner\n";
	}

	// 2 but no 0 1 3; one possible
	if (p1.x == -1 && p2.x == -1 && p3.x != -1 && p4.x == -1) {
		outputFile << "southeast corner\n";
	}

	// 3 but no 0 1 2; one possible
	if (p1.x == -1 && p2.x == -1 && p3.x == -1 && p4.x != -1) {
		outputFile << "southwest corner\n";
	}

	// all 1 of 4 known accounted for
	// now all or nothing

	// no knowns
	if (p1.x == -1 && p2.x == -1 && p3.x == -1 && p4.x == -1) {
		outputFile << "no drone found\n";
	}

	// all knowns; this is the hard part
	if (p1.x != -1 && p2.x != -1 && p3.x != -1 && p4.x != -1) {
		outputFile << "all cams found\n";

		// camera viewport is 1280 * 960
		
	}

	outputFile.close();

}