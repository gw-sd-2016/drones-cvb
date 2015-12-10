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

void cleanCoordinates(int camera) {
	ifstream outputFile;
	string line;
	string out = "output" + to_string(camera) + ".txt";
	cout << "Opening " << out << "\n";
	outputFile.open(out);

	int length = outputFile.tellg();
	Point2f *points = new Point2f[length];
	string delimiter1 = ".";
	string delimiter2 = ",";
	size_t pos = 0;
	string token;

	if (outputFile.is_open()) {
		cout << "Reading contents of file \n";
		while(!outputFile.eof()) {
			getline(outputFile, line);
			cout << line << "\n";
			// first delimiting
			while ((pos = line.find(delimiter1)) != string::npos) {
				token = line.substr(0, pos);
				cout << token << endl;
				line.erase(0, pos + delimiter1.length());
			}
		}
	}

	outputFile.close();

}
