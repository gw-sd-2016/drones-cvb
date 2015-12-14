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
	vector<Point2f> points;
	string delimiter1 = ".";
	string delimiter2 = ",";
	size_t pos = 0;
	size_t pos2 = 0;
	string token;
	int tempX, tempY;

	if (outputFile.is_open()) {
		cout << "Reading contents of file \n";
		while(!outputFile.eof()) {
			getline(outputFile, line);
			// first delimiting
			if ((pos = line.find(delimiter1)) != string::npos) {	// if 1st delimiter found in line
				token = line.substr(0, pos);						// set token to first text until delimiter
				cout << "first: " << token << endl;					// debugging print line
				tempX = atoi(token.c_str());						// saves token in tempX
				line.erase(0, pos + delimiter1.length());			// erases section up through delimiter
			}

			// second delimiting
			if (((pos2 = line.find(delimiter2)) != string::npos) && ((pos = line.find(delimiter1)) != string::npos)) {	// if 1st and 2nd delimiter both found in line
				token = line.substr(pos2 + delimiter2.length(), pos);													// should be just between the comma and the period
				cout << "second: " << token << endl;																	// debugging print line
				tempY = atoi(token.c_str());																			// saves token in tempY
			}																											// don't need to erase for last number

			cout << "tempX: " << tempX << ". tempY: " << tempY << endl;
			points.push_back(Point2f(tempX, tempY));
		}
	}

	outputFile.close();

	cout << "Printing the vector" << endl;
	for (int i = 0; i < points.size(); ++i) {
		cout << points[i].x << ", " << points[i].y << endl;
	}

}
