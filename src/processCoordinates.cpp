#include <stdio.h>
#include <iostream>
#include <fstream>

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

int processCoordinates(int camera) {

	ifstream outputFile;
	string line;
	string out = "output" + to_string(camera) + ".txt";
	cout << "Opening " << out << "\n";
	outputFile.open(out, ios::ate);

	// length of file stored to variable
	int length = outputFile.tellg();	
	
	// array of Point2fs of size file length
	Point2f *points = new Point2f[length];

	if (outputFile.is_open()) {
		cout << "here";
		while(getline(outputFile, line)) {
			cout << line << "\n";
			cout << "there";
		}
	}

	outputFile.close();

	delete [] points;
	return 0;

}