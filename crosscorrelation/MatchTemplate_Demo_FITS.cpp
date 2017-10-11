/**
 *  compatible with opencv2
 *  http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/template_matching/template_matching.html
 */
#include "fitsio.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;
using namespace cv;

/// Global CV Variables
Mat img;
Mat templ;
Mat result;
char *image_window = "Source Image";
char *result_window = "Result window";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod(int, void *);
double **fitsread(const char *file, int *sizeX, int *sizeY);
float *double2float(double **pd, int x, int y);

/** @function main */
int main(int argc, char **argv) {
#if 0
  /// Load image and template
  img = imread(argv[1], 1);
  templ = imread(argv[2], 1);
#else
  int x, y;
  double **fitsImg;
  float *ffitsImg;
  fitsImg = fitsread(argv[1], &x, &y);
  {
    ffitsImg = double2float(fitsImg, x, y);
    cv::Mat tmp(y, x, CV_32FC1, ffitsImg);
    tmp.copyTo(img);
  }

  fitsImg = fitsread(argv[2], &x, &y);
  {
    ffitsImg = double2float(fitsImg, x, y);
    cv::Mat tmp(y, x, CV_32FC1, ffitsImg);
    tmp.copyTo(templ);
  }
#endif

#if 0
    int nr = templ.rows;                    // number of rows
    int nc = templ.cols * templ.channels(); // total number of elements per line
    for (int j = 0; j < nr; j++) {
      float *data = templ.ptr<float>(j);
      for (int i = 0; i < nc; i++) {
        printf("%.0f ", data[i]);
      }
      printf("\n");
    }
#endif

  /// Create windows
  namedWindow(image_window, CV_WINDOW_AUTOSIZE);
  namedWindow(result_window, CV_WINDOW_AUTOSIZE);

  /// Create Trackbar
  char *trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM "
                         "CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM "
                         "COEFF NORMED";
  createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar,
                 MatchingMethod);

  MatchingMethod(0, 0);

  waitKey(0);
  return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod(int, void *) {
  /// Source image to display
  Mat img_display;
  img.copyTo(img_display);

  /// Create the result matrix
  int result_cols = img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create(result_rows, result_cols, CV_64FC1);

  /// Do the Matching and Normalize
  matchTemplate(img, templ, result, match_method);
  normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

  /// Localizing the best match with minMaxLoc
  double minVal;
  double maxVal;
  Point minLoc;
  Point maxLoc;
  Point matchLoc;

  minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all
  /// the other methods, the higher the better
  if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
    matchLoc = minLoc;
  } else {
    matchLoc = maxLoc;
  }

  /// Show me what you got
  rectangle(img_display, matchLoc,
            Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
            Scalar::all(255), 2, 8, 0);
  rectangle(result, matchLoc,
            Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
            Scalar::all(255), 2, 8, 0);

  imshow(image_window, img_display);
  imshow(result_window, result);

  return;
}

double **fitsread(const char *file, int *sizeX, int *sizeY) {
  /// Global fitsio Variables
  fitsfile *fptr; /* FITS file pointer, defined in fitsio.h */
  int status = 0; /* CFITSIO status value MUST be initialized to zero! */
  int bitpix, naxis, ii, anynul;
  long naxes[2] = {1, 1}, fpixel[2] = {1, 1};
  double *pixels;
  char format[20], hdformat[20];

  double **fitsData = NULL;

  if (!fits_open_file(&fptr, file, READONLY, &status)) {
    if (!fits_get_img_param(fptr, 2, &bitpix, &naxis, naxes, &status)) {
      if (naxis > 2 || naxis == 0)
        printf("Error: only 1D or 2D images are supported\n");
      else {
        /* get memory for 1 row */
        pixels = (double *)malloc(naxes[0] * sizeof(double));

        if (pixels == NULL) {
          printf("Memory allocation error\n");
          return NULL;
        }

        /* allocate memory */
        fitsData = new double *[naxes[1]];
        assert(fitsData != NULL);
        for (int i = 0; i < naxes[1]; i++) {
          fitsData[i] = new double[naxes[0]];
          assert(fitsData[i] != NULL);
        }
        *sizeX = naxes[0];
        *sizeY = naxes[1];

        if (bitpix > 0) { /* set the default output format string */
          strcpy(hdformat, " %7d");
          strcpy(format, " %7.0f");
        } else {
          strcpy(hdformat, " %15d");
          strcpy(format, " %15.5f");
        }
#if 0
        printf("\n      "); /* print column header */
        for (ii = 1; ii <= naxes[0]; ii++)
          printf(hdformat, ii);
        printf("\n"); /* terminate header line */
#endif
        /* loop over all the rows in the image, top to bottom */
        for (fpixel[1] = naxes[1]; fpixel[1] >= 1; fpixel[1]--) {
          if (fits_read_pix(fptr, TDOUBLE, fpixel, naxes[0], NULL, pixels, NULL,
                            &status)) /* read row of pixels */
            break;                    /* jump out of loop on error */
          memcpy(fitsData[naxes[1] - fpixel[1]], pixels,
                 naxes[0] * sizeof(double));

#if 0
          printf(" %4d ", fpixel[1]); /* print row number */
          for (ii = 0; ii < naxes[0]; ii++)
            printf(format, pixels[ii]); /* print each value  */
          printf("\n");                 /* terminate line */
#endif
        }
        free(pixels);
      }
    }
    fits_close_file(fptr, &status);
  }

  if (status)
    fits_report_error(stderr, status); /* print any error message */
  return fitsData;
}

float *double2float(double **pd, int x, int y) {
  float *pf;
  pf = new float[x * y];
  assert(pf != NULL);

  for (int i = 0; i < y; i++) {
    for (int j = 0; j < x; j++) {
      pf[i * x + j] = pd[i][j];
    }
  }
  return pf;
}
