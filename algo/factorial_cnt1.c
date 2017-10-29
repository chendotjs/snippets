/**
 * given integer n and prime a, how many does n! contains a.
 */

#include <stdio.h>

int solution1(int n, int a);
int solution2(int n, int a);

int main(int argc, char const *argv[]) {
  int n, a;
  while (scanf("%d%d", &n, &a) != EOF) {
    printf("solution1 %d\n", solution1(n, a));
    printf("solution2 %d\n", solution2(n, a));
  }
  return 0;
}

int solution1(int n, int a) {
  int cnt = 0;
  int p = a;
  while (n / p > 0) {
    cnt += n / p;
    p *= a;
  }
  return cnt;
}

int solution2(int n, int a) {
  if (n == 1)
    return 0;
  int nCnt = 0, nn = n;
  while (n % a == 0) {
    n = n / a;
    nCnt++;
  }
  return nCnt + solution2(nn - 1, a);
}
