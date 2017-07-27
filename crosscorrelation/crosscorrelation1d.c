/**
 * http://paulbourke.net/miscellaneous/correlate/
 */

#include <math.h>
#include <stdio.h>

int x[] = {1, 1, 1, 2, 3, 4, 5, 6, 1, 1, 1, 1};
int y[] = {1, 1, 2, 3, 4, 5, 6, 1, 1, 1, 1, 1};
int n = sizeof(x) / sizeof(int);
int maxdelay = 3;

int main(int argc, char const *argv[]) {

  int i, j, delay;
  double mx, my, sx, sy, sxy, denom, r;

  /* Calculate the mean of the two series x[], y[] */
  mx = 0;
  my = 0;
  for (i = 0; i < n; i++) {
    mx += x[i];
    my += y[i];
  }
  mx /= n;
  my /= n;

  /* Calculate the denominator */
  sx = 0;
  sy = 0;
  for (i = 0; i < n; i++) {
    sx += (x[i] - mx) * (x[i] - mx);
    sy += (y[i] - my) * (y[i] - my);
  }
  denom = sqrt(sx * sy);

  /* Calculate the correlation series */
  for (delay = -maxdelay; delay < maxdelay; delay++) {
    sxy = 0;
    for (i = 0; i < n; i++) {
      j = i + delay;
      if (j < 0 || j >= n)
        continue;
      else
        sxy += (x[i] - mx) * (y[j] - my);
      /* Or should it be (?)
      if (j < 0 || j >= n)
         sxy += (x[i] - mx) * (-my);
      else
         sxy += (x[i] - mx) * (y[j] - my);
      */
    }
    r = sxy / denom;

    /* r is the correlation coefficient at "delay" */
    printf("delay:%3d\tcoefficient:%lf\n", delay, r); // find the largest one
  }
  return 0;
}
