/* https://stackoverflow.com/questions/44453088/how-to-convert-c-array-to-opencv-mat */
#include <iostream>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
#if 0
	// only this works
  double x[5][5] = {{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}};
#else
	// this does not work
	double **x;
	x = new double* [5];
	for (int i = 0; i < 5; i++) {
		x[i] = new double[5];
	}
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      x[i][j] = i * 5 + j;
    }
  }
#endif
  cv::Mat A(5, 5, CV_64FC1, x);

  printf("Mat channel: %d\n", A.channels());

  int nr = A.rows;                // number of rows
  int nc = A.cols * A.channels(); // total number of elements per line
  for (int j = 0; j < nr; j++) {
    double *data = A.ptr<double>(j);
    for (int i = 0; i < nc; i++) {
      printf("%.0lf ", data[i]);
    }
  }
  printf("\n");

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%.0lf ", x[i][j]);
    }
  }
  printf("\n");

  return 0;
}
