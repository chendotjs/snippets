/**
 * Reference: http://bug-gnulib.gnu.narkive.com/t3LuJTU7/making-int-strlen-bound-tight-for-unsigned-types
 */

#include <stdio.h>

#define CHAR_BIT (8)
#define INT_STRLEN_BOUND(t) ((sizeof(t) * CHAR_BIT - 1) * 146 / 485 + 2)

#define PRINT_RESULT(t)                                                        \
  do {                                                                         \
    printf(#t ": %d\n", INT_STRLEN_BOUND(t));                                  \
  } while (0)

int main(int argc, char const *argv[]) {
  PRINT_RESULT(char);
  PRINT_RESULT(int);
  PRINT_RESULT(long);
  int a = 10, b = 100000;
  PRINT_RESULT(a);
  PRINT_RESULT(b);
  return 0;
}
