#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

void to3D(vector<Point2f> cam0, vector<Point2f> cam1, vector<Point2f> cam2, vector<Point2f> cam3) {

	// vector of points for each camera

	// go through every point in the vector
	for (int i = 0; i < smallestVector; i++) {

		// no data points on 1 of 4 quadrants

		// no 0 but 1 2 3; three quadrants possible
		if (cam0[i].x == -1 && cam1[i].x != -1 && cam2[i].x != -1 && cam3[i].x != -1) {

		}

		// no 1 but 0 2 3; three quadrants possible
		if (cam0[i].x != -1 && cam1[i].x == -1 && cam2[i].x != -1 && cam3[i].x != -1) {

		}

		// no 2 but 0 1 3; three quadrants possible
		if (cam0[i].x != -1 && cam1[i].x != -1 && cam2[i].x == -1 && cam3[i].x != -1) {

		}

		// no 3 but 0 1 2; three quadrants possible
		if (cam0[i].x != -1 && cam1[i].x != -1 && cam2[i].x != -1 && cam3[i].x == -1) {

		}

		// all 1 of 4 accounted for
		// now 2 of 2

		// 0 1 but no 2 3; half possible
		if (cam0[i].x != -1 && cam1[i].x != -1 && cam2[i].x == -1 && cam3[i].x == -1) {

		}

		// 1 2 but no 0 3; half possible
		if (cam0[i].x == -1 && cam1[i].x != -1 && cam2[i].x != -1 && cam3[i].x == -1) {

		}

		// 2 3 but no 0 1; half possible
		if (cam0[i].x == -1 && cam1[i].x == -1 && cam2[i].x != -1 && cam3[i].x != -1) {

		}

		// 0 3 but no 1 2; half possible
		if (cam0[i].x != -1 && cam1[i].x == -1 && cam2[i].x == -1 && cam3[i].x != -1) {

		}

		// 0 2 but no 1 3; unknown outcome
		if (cam0[i].x != -1 && cam1[i].x == -1 && cam2[i].x != -1 && cam3[i].x == -1) {

		}

		// 1 3 but no 0 2; unknown outcome
		if (cam0[i].x == -1 && cam1[i].x != -1 && cam2[i].x == -1 && cam3[i].x != -1) {

		}

		// all 2 of 4 accounted for
		// now 1 of 4 known

		// 0 but no 1 2 3; one possible
		if (cam0[i].x != -1 && cam1[i].x == -1 && cam2[i].x == -1 && cam3[i].x == -1) {

		}

		// 1 but no 0 2 3; one possible
		if (cam0[i].x == -1 && cam1[i].x != -1 && cam2[i].x == -1 && cam3[i].x == -1) {

		}

		// 2 but no 0 1 3; one possible
		if (cam0[i].x == -1 && cam1[i].x == -1 && cam2[i].x != -1 && cam3[i].x == -1) {

		}

		// 3 but no 0 1 2; one possible
		if (cam0[i].x == -1 && cam1[i].x == -1 && cam2[i].x == -1 && cam3[i].x != -1) {

		}

		// all 1 of 4 known accounted for
		// now all or nothing

		// no knowns
		if (cam0[i].x == -1 && cam1[i].x == -1 && cam2[i].x == -1 && cam3[i].x == -1) {

		}

		// all knowns; this is the hard part
		if (cam0[i].x != -1 && cam1[i].x != -1 && cam2[i].x != -1 && cam3[i].x != -1) {

		}


	}

}