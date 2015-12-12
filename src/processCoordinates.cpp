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
	string token;
	int tempX, tempY;
	int currentIndex = 0;

	if (outputFile.is_open()) {
		cout << "Reading contents of file \n";
		while(!outputFile.eof()) {
			getline(outputFile, line);
			// first delimiting
			while ((pos = line.find(delimiter1)) != string::npos) {
				token = line.substr(0, pos);
				cout << token << endl;
				tempX = atoi(token.c_str());			// saves token in tempX
				line.erase(0, pos + delimiter1.length());
				// second delimiting
				while ((pos = line.find(delimiter2)) != string:: npos) {
					if (line.substr(0, 1) != "0") {	// second time only, avoids zeros
						token = line.substr(0, pos);
						cout << token << endl;
						tempY = atoi(token.c_str());	// saves token in tempY
					}
					line.erase(0, pos + delimiter2.length());
				}
			}

			points.push_back(Point2f(tempX, tempY));
			currentIndex++;

		}
	}

	outputFile.close();

	cout << "Printing the array" << endl;


}
