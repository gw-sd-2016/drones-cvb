#include <stdio.h>
#include <stdlib.h>
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

vector<Point2f> cleanCoordinates(int camera) {
	ifstream outputFile;
	string line;
	string out = "camera" + to_string(camera) + ".txt";
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

	/*
		Delimits input file of coordinates into array of points
	*/
	if (outputFile.is_open()) {
		cout << "Reading contents of file \n";
		while(!outputFile.eof()) {
			getline(outputFile, line);
			// first delimiting
			if ((pos = line.find(delimiter1)) != string::npos) {														// if 1st delimiter found in line
				token = line.substr(0, pos);																			// set token to first text until delimiter
				cout << "first: " << token << endl;																		// debugging print line
				tempX = atoi(token.c_str());																			// saves token in tempX
				line.erase(0, pos + delimiter1.length());																// erases section up through delimiter
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

	return points;

	/*
		Begin the cleaning up portion

		Preliminary idea is for every coordinate, look at the next and the previous coordinates. If the examined one is outside of some distance of **each or average** 
		of the other two, mark it for deletion. Will disregard first and last coordinates because they don't have previous or next respectively.
	*/

	int distanceVar = 10;							// distance variable; this will be played with
	
	Point2f prev, next, average;					// previous, next, average points
	vector<int> markers;
	for (int i = 0; i < points.size(); ++i) {		// for every point in vector
		if ((i > 0) && (i < points.size() - 1)) {	// between 0 and 2nd to last in points
			prev = points[i - 1];					// set the previous point
			next = points[i + 1];					// set the next point

			// try average first
			average = Point2f((prev.x + next.x)/2, (prev.y + next.y)/2);		// average of individual x and y coordinates from prev and next
			cout << "average: " << average.x << ", " << average.y << endl;
			cout << "abs: " << abs(points[i].x - average.x) << ", " << abs(points[i].y - average.y) << endl;
			if ((abs(points[i].x - average.x) > distanceVar) || (abs(points[i].y - average.y) > distanceVar)) {
				markers.push_back(1);						// mark point for deletion
				cout << "marking the point for deletion... " << endl;
			}
			else {
				markers.push_back(0);						// mark point as safe
				cout << "marking the point to save... " << endl;
			}
		}
		else { markers.push_back(2); }						// A 2 indicates the point has been ignored.

		cout << "\n" << endl;
	}

	cout << "Printing the markers" << endl;
	for (int i = 0; i < markers.size(); ++i) {
		cout << markers[i] << endl;
	}

}
