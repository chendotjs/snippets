/**
 * simplest version of  Discrete Fourier Transform
 * http://www.jot.fm/contents.html?query=Discrete+Fourier
 * part 1
 *
 * Note: just a test example,
 */

#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#define PI 3.1415926

class FFT {
public:
  double *r_data;
  double *i_data;
  double *psd;
  double *iv;
  int N;

  double *dft(double v[], int N) {
    double t_img, t_real;
    double twoPikOnN;
    double twoPijkOnN;

    N = (int)log2(N);
    N = 1 << N;
    this->N = N;

    double twoPiOnN = 2 * PI / N;

    r_data = new double[N];
    i_data = new double[N];
    psd = new double[N];

    memset(r_data, 0, sizeof(double) * N);
    memset(i_data, 0, sizeof(double) * N);

    printf("Executing DFT on %d points...\n", N);
    for (int k = 0; k < N; k++) {
      twoPikOnN = twoPiOnN * k;
      for (int j = 0; j < N; j++) {
        twoPijkOnN = twoPikOnN * j;
        r_data[k] += v[j] * cos(twoPijkOnN);
        i_data[k] -= v[j] * sin(twoPijkOnN);
      }
      r_data[k] /= N;
      i_data[k] /= N;
      psd[k] = r_data[k] * r_data[k] + i_data[k] * i_data[k];
    }
    return psd;
  }

  double *idft() {
    int N = this->N;
    double twoPiOnN = 2 * PI / N;
    double twoPikOnN;
    double twoPijkOnN;
    iv = new double[N];

    printf("Executing IDFT on %d points...\n", N);

    for (int k = 0; k < N; k++) {
      twoPikOnN = twoPiOnN * k;
      iv[k] = 0;
      for (int j = 0; j < N; j++) {
        twoPijkOnN = twoPikOnN * j;
        iv[k] += r_data[j] * cos(twoPijkOnN) - i_data[j] * sin(twoPijkOnN);
      }
    }
    return iv;
  }
  ~FFT() {
    printf("FFT destructed...\n");
    delete[] r_data;
    delete[] i_data;
    delete[] psd;
    delete[] iv;
  }
};

int main(int argc, char const *argv[]) {
  // example1
  {
    FFT fft;
    double x[] = {0, 1, 2, 3, 4, 5, 6, 7};
    int N = sizeof(x) / sizeof(double);

    fft.dft(x, N);
    fft.idft();
    printf("j\tx[j]\tre[j]\t\tim[j]\t\tv[j]\n");
    for (int j = 0; j < N; j++) {
      printf("%d\t%.0lf\t%lf\t%lf\t%lf\n", j, x[j], fft.r_data[j],
             fft.i_data[j], fft.iv[j]);
    }
  }

  // example2 delta_function
  {
    FFT fft;
    double x[8] = {0};
    x[0] = 1;
    int N = sizeof(x) / sizeof(double);

    fft.dft(x, N);
    fft.idft();
    printf("j\tx[j]\tre[j]\t\tim[j]\t\tv[j]\n");
    for (int j = 0; j < N; j++) {
      printf("%d\t%.0lf\t%lf\t%lf\t%lf\n", j, x[j], fft.r_data[j],
             fft.i_data[j], fft.iv[j]);
    }
  }

  // example3 ϵ function
  {
    FFT fft;
    double x[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    int N = sizeof(x) / sizeof(double);

    fft.dft(x, N);
    fft.idft();
    printf("j\tx[j]\tre[j]\t\tim[j]\t\tv[j]\n");
    for (int j = 0; j < N; j++) {
      printf("%d\t%.0lf\t%lf\t%lf\t%lf\n", j, x[j], fft.r_data[j],
             fft.i_data[j], fft.iv[j]);
    }
  }
  return 0;
}
