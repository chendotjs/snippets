/**
 * 为什么rin和in会相差len倍?
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fftw3.h"
#pragma comment(lib, "libfftw3-3.lib") // double版本
// #pragma comment(lib, "libfftw3f-3.lib")// float版本
// #pragma comment(lib, "libfftw3l-3.lib")// long double版本

#define PI 3.1415926

int main() {
  int len = 20;
  double *in = NULL;
  double *rin = NULL;
  // 如果要使用float版本,需先引用float版本的lib库,然后在fftw后面加上f后缀即可.
  fftw_complex *out = NULL; // fftwf_complex --> 即为float版本
  fftw_plan p;
  in = (double *)fftw_malloc(sizeof(double) * len);
  rin = (double *)fftw_malloc(sizeof(double) * len);
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * len);
  double dx = 1.0 / len;

  // 输入纯实数
  for (int i = 0; i < len; i++) {
    in[i] = sin(2 * PI * dx * i) + sin(4 * PI * dx * i) + 5;
    printf("%2.2lf ", in[i]);
  }
  printf("\n\n");

  // 傅里叶变换
  p = fftw_plan_dft_r2c_1d(len, in, out, FFTW_ESTIMATE);
  fftw_execute(p);

  // 输出幅度谱
  // the input and output arrays are of different sizes and types: the input is
  // n real numbers, while the output is n/2+1 complex numbers (the
  // non-redundant outputs)
  for (int i = 0; i < len; i++) {
    double len = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
    printf("%2.2lf ", len);
  }
  printf("\n\n");

  fftw_plan rp;
  rp = fftw_plan_dft_c2r_1d(len, out, rin, FFTW_ESTIMATE);
  fftw_execute(rp);

  for (int i = 0; i < len; i++) {
    printf("%2.2lf ", rin[i]);
  }

  // 释放资源
  fftw_destroy_plan(p);
  fftw_destroy_plan(rp);
  fftw_free(in);
  fftw_free(out);
  fftw_free(rin);

  return 0;
}
