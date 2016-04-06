#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "to3D.cpp"

#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

Point2f stringtoPoint(string line) {

	// incoming format of (x,y)
	double a = stod(line.substr(1, 10));
	double b = stod(line.substr(12, 21));
	printf("a: %f\n", a);
	printf("b: %f\n", b);

	return Point2f(a, b);

}

Point2f delimiter(string str) {

	string delim = ",";
	size_t pos = 0;
	string token;

	pos = str.find(delim);

	string x = str.substr(0, pos);
	string y = str.substr(pos + delim.length(), str.length());

	return Point2f(stoi(x), stoi(y));
}

Point3f processCoordinates(string one, string two, string three, string four) {

	Point2f p1, p2, p3, p4;

	if ((one.find("-1") != string::npos) || (two.find("-1") != string::npos)) {
		return Point3f(-1, -1, -1);
	}

	// run coordinate string through delimiter if the coordinate was found
	if (one.find("-1") == string::npos) {
		p1 = delimiter(one);
		//printf("Camera one delimited %s into %f, %f\n", one.c_str(), p1.x, p1.y);
	}

	if (two.find("-1") == string::npos) {
		p2 = delimiter(two);
		//printf("Camera two delimited %s into %f, %f\n", two.c_str(), p2.x, p2.y);
	}

	if (three.find("-1") == string::npos) {
		p3 = delimiter(three);
		//printf("Camera three delimited %s into %f, %f\n", three.c_str(), p3.x, p3.y);
	}

	if (four.find("-1") == string::npos) {
		p4 = delimiter(four);
		//printf("Camera four delimited %s into %f, %f\n", four.c_str(), p4.x, p4.y);
	}

	Point3f currentLocation = to3D(p1, p2, p3, p4);

	return currentLocation;
}

