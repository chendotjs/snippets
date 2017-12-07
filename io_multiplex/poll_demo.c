#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define oops(m, x)                                                             \
  do {                                                                         \
    perror(m);                                                                 \
    exit(x);                                                                   \
  } while (0)

void showdata(const char *fname, int fd);

int main(int argc, char const *argv[]) {
  int fd1, fd2;
  struct pollfd fds[2];
  int timeout = 1000 * atoi(argv[3]);
  int retval;

  if (argc != 4) {
    fprintf(stderr, "usage:%s file file timeout\n", argv[0]);
    exit(1);
  }

  if ((fd1 = open(argv[1], O_RDONLY)) == -1)
    oops(argv[1], 2);
  if ((fd2 = open(argv[2], O_RDONLY)) == -1)
    oops(argv[2], 3);
  fds[0].fd = fd1;
  fds[1].fd = fd2;
  fds[0].events = POLLIN;
  fds[1].events = POLLIN;

  while (1) {
    retval = poll(fds, sizeof(fds) / sizeof(struct pollfd), timeout);
    if (retval == -1)
      oops("poll", 4);
    if (retval > 0) {
      if (fds[0].revents & POLLIN)
        showdata(argv[1], fd1);
      if (fds[1].revents & POLLIN)
        showdata(argv[2], fd2);
    } else
      printf("no input buffer after %d seconds\n", atoi(argv[3]));
  }
  return 0;
}

void showdata(const char *fname, int fd) {
  char buf[BUFSIZ];
  int n;
  printf("%s: ", fname);
  fflush(stdout);
  n = read(fd, buf, BUFSIZ);
  if (n == -1)
    oops(fname, 5);
  write(1, buf, n);
  write(1, "\n", 1);
}
