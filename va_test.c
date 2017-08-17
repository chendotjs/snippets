/**
 * `gcc va_test.c -g3 -v` to figure out which header is used
 */
#include <stdarg.h>
#include <stdio.h>

// fmt implies the numbers of args
void foo(char *fmt, ...) {
  va_list ap;
  int d;
  char c, *s;

  va_start(ap, fmt);
  while (*fmt)
    switch (*fmt++) {
    case 's': /* string */
      s = va_arg(ap, char *);
      printf("string %s\n", s);
      break;
    case 'd': /* int */
      d = va_arg(ap, int);
      printf("int %d\n", d);
      break;
    case 'c': /* char */
      /* need a cast here since va_arg only
         takes fully promoted types */
      c = (char)va_arg(ap, int);
      printf("char %c\n", c);
      break;
    }
  va_end(ap);
  printf("-------------------\n");
}

int main(int argc, char const *argv[]) {
  foo("sdc", "hello", 360, 'C');
  foo("sdc", "hello", 360, 'C', "bye");
  foo("sddc", "hello", 360, 100, 'C');
  return 0;
}
