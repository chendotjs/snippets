/**
 * rough bigIntegers
 */
#include <cstdio>
#include <cstring>
using namespace std;

#define LENGTH 10010

struct bigInteger {
  char digit[LENGTH];
  int size;

  void init() {
    memset(digit, 0, sizeof(digit));
    size = 0;
  }

  void set(const char *str) {
    init();
    int len = strlen(str);
    for (int i = len - 1; i >= 0; i--) {
      digit[size++] = str[i] - '0';
    }
  }

  void output() {
    for (int i = size - 1; i >= 0; i--) {
      printf("%d", digit[i]);
    }
    printf("\n");
  }

  bigInteger operator*(int x) const {
    bigInteger ans;
    ans.init();
    int carry = 0;
    for (int i = 0; i < size; i++) {
      long int tmp = x * digit[i] + carry;
      carry = tmp / 10;
      ans.digit[ans.size++] = tmp % 10;
    }
    while (carry) {
      ans.digit[ans.size++] = carry % 10;
      carry /= 10;
    }
    return ans;
  }

  bigInteger operator+(const bigInteger &x) const {
    bigInteger ans;
    ans.init();

    int carry = 0;
    int maxSize = size > x.size ? size : x.size;
    for (int i = 0; i < maxSize; i++) {
      int tmp = digit[i] + x.digit[i] + carry;
      carry = tmp / 10;
      ans.digit[ans.size++] = tmp % 10;
    }

    if (carry)
      ans.digit[ans.size++] = carry;
    return ans;
  }

  bigInteger operator-(const bigInteger &x) const {
    bigInteger ans;
    ans.init();

    int carry = 0;
    for (int i = 0; i < size; i++) {
      int cur = digit[i] - carry;
      if (cur < x.digit[i]) {
        carry = 1;
        ans.digit[ans.size++] = 10 + cur - x.digit[i];
      } else {
        carry = 0;
        ans.digit[ans.size++] = cur - x.digit[i];
      }
    }
    for (int i = 0; i < size; i++) { // 去掉前置0
      if (ans.digit[i] != 0)
        ans.size = i;
    }
    ans.size++;
    return ans;
  }

  bigInteger operator/(int x) const {
    bigInteger ans;
    ans.init();

    int remainer = 0;
    for (int i = size - 1; i >= 0; i--) {
      int tmp = remainer * 10 + digit[i];
      int t = tmp / x;
      int r = tmp % x;

      remainer = r;
      ans.digit[i] = t;
    }

    for (int i = size - 1; i >= 0; i--) {
      if (ans.digit[i] != 0) {
        ans.size = i + 1;
        break;
      }
      ans.size = 1; // in case result = 0
    }
    return ans;
  }

  int operator%(int x) const {
    int remainer = 0;
    for (int i = size - 1; i >= 0; i--) {
      int tmp = remainer * 10 + digit[i];
      int t = tmp / x;
      int r = tmp % x;
      remainer = r;
    }
    return remainer;
  }
};

bigInteger a, b, c;

int main() {
  {
    a.set("1234");
    b.set("99999");
    c = a + b;
    c.output();
  }

  {
    a.set("1000");
    b.set("123");
    c = a - b;
    c.output();
  }

  {
    a.set("1234");
    c = a * 123;
    c.output();
  }

  {
    a.set("1234");
    c = a / 12;
    c.output();
  }

  {
    a.set("1234");
    printf("%d\n", a % 12);
  }
  return 0;
}
