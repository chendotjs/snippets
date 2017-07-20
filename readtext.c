/**
 * `fgets` Reading stops when a newline character is found, at end-of-file or
 * error.
 *  所以缓冲区足够大的话，每次确实读完一行；如果缓冲区比较小，一行读不完缓冲区就满了。
 *
 *  `fread`读固定的长度（如果文件剩余的部分大于缓冲区），要检查实际读了多少长度。
 */

#include <stdio.h>
#include <stdlib.h>

#define SHOW_LINE_END 1
#define LENGTH 128 // try changing LENGTH to a smaller size

#define EOL "\033[33mEOL\033[0m"

void read1(const char *file) {
  FILE *fp = fopen(file, "r");
  char line_buffer[LENGTH];
  while (fgets(line_buffer, LENGTH, fp) !=
         NULL) { // at most LENGTH, append '\0' to the end
    fputs(line_buffer, stdout);
#if SHOW_LINE_END
    printf(EOL);
#endif
  }
}

void read2(const char *file) {
  FILE *fp = fopen(file, "r");
  char line_buffer[LENGTH];
  int readn;
  while ((readn = fread(line_buffer, 1, LENGTH, fp)) > 0) {
    fwrite(line_buffer, 1, readn, stdout);
#if SHOW_LINE_END
    printf("%d%s", readn, EOL);
#endif
  }
}

int main(int argc, char const *argv[]) {
  if (argc <= 1) {
    exit(0);
  }

  read1(argv[1]);
  printf("\n\n\n");
  read2(argv[1]);

  return 0;
}
