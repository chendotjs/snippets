#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define K (1024)
#define M (1024 * K)

int main(int argc, char const *argv[]) {
  char *memo = (char *)malloc(1024 * M);
  memset(memo, 'c', 1024 * M);

  int r = fork();
  if (r > 0) {
    printf("father: %d\n", getpid());
    while (1)
      sleep(1);
  } else if (r == 0) {
    printf("child: %d\n", getpid());
    while (1)
      sleep(1);
  } else {
    perror("fork failed");
    exit(1);
  }

  return 0;
}
