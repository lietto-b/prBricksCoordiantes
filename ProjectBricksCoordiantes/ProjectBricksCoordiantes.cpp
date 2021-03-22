#include <iostream>
#include <fstream>
#include <string>

#include <opencv/cv.h>
#include <opencv2/imgproc.hpp>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
	Mat img = imread("DSC.png");
	Mat edged, kernel, closed, gray, bw, smoth;
	GaussianBlur(img, smoth, Size(7, 7), 0);
	Canny(smoth, edged, 0, 255);

	kernel = getStructuringElement(MORPH_RECT, Point(3, 3));
	morphologyEx(edged, closed, MORPH_CLOSE, kernel);

	vector<vector<Point>> cnts;
	findContours(closed, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	Mat src = imread("DSC_0007.jpg");
	double peri;

	vector<vector<Point>> approxs;
	vector<Point> approx;

	int total = 0;
	for (vector<Point>c : cnts) {
		peri = arcLength(c, true);
		approxPolyDP(c, approx, 0.02 * peri, true);

		if (approx.size() == 4 && peri > 900) {
			approxs.push_back(approx);
			total++;
		}
	}

	ofstream f("kirp.txt");

	for (vector<Point> c : approxs) {
		f << c << endl;
	}

	drawContours(src, approxs, -1, Scalar(255, 0, 0), 5);
	drawContours(img, approxs, -1, Scalar(0, 255, 0), 5);
	cout << "Count of kirp: " << total << endl;

	imwrite("smoth.png", smoth);
	imwrite("edged.png", edged);
	imwrite("closed.png", closed);
	imwrite("outimg.png", src);
	imwrite("outbin.png", img);

	system("pause");
	return 0;
}

