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

int processCoordinates(string one, string two, string three, string four) {

	Point2f p1, p2, p3, p4;

	// run coordinate string through delimiter if the coordinate was found
	if (one.find("-1") == string::npos) {
		p1 = delimiter(one);
		printf("Camera one delimited %s into %f, %f\n", one.c_str(), p1.x, p1.y);

		/* NEW CODE */

		// test of one camera's intrinsics
		double FRONT_CAMERA_MATRIX[3][3] = {{1.4049474067879219e+03, 0, 6.3950000000000000e+02}, 
											{0, 1.4049474067879219e+03, 4.7950000000000000e+02}, 
											{0, 0, 1}};


		// convert 2D point into homogenous point
		Matx31f = hom_pt(p1.x, p1.y, 1);

		// multiply by inverse of camera intrinsics matrix
		hom_pt = FRONT_CAMERA_MATRIX.inv() * hom_pt;		// this is the line that actually puts
															// the coordinate into world coords

		Point3f origin(0, 0, 0);
		Point3f direction(hom_pt(0), hom_pt(1), hom_pt(2);

		// normalize the direction to get unit vector
		direction *= 1/norm(direction);

		/*
			Origin and direction now define the ray in the world space that corresponds to
			the image point. Note that the origin is centered on the camera. Use camera pose
			to transform to a different origin. Distortion coefficients map from the camera
			to the pinhole camera model and should be used at the very beginning to find the
			actual 2D coordinates. Then:

			1. Undistort 2D coordinate with distortion coefficients
			2. Convert to ray (above)
			3. Move that ray to desired coordinate system
		*/

	}

	if (two.find("-1") == string::npos) {
		p2 = delimiter(two);
		printf("Camera two delimited %s into %f, %f\n", two.c_str(), p2.x, p2.y);
	}

	if (three.find("-1") == string::npos) {
		p3 = delimiter(three);
		printf("Camera three delimited %s into %f, %f\n", three.c_str(), p3.x, p3.y);
	}

	if (four.find("-1") == string::npos) {
		p4 = delimiter(four);
		printf("Camera four delimited %s into %f, %f\n", four.c_str(), p4.x, p4.y);
	}

	// currently only programed for two cameras


	// no clue yet how to return this
	to3D(p1, p2, p3, p4);

	return 0;
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
