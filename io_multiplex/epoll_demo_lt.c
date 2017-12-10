#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define oops(m, x)                                                             \
  do {                                                                         \
    perror(m);                                                                 \
    exit(x);                                                                   \
  } while (0)

#define MAKE_EVENT(NAME, IDX)                                                  \
  struct epoll_event NAME##IDX;                                                \
  (NAME##IDX).events = EPOLLIN;                                                \
  (NAME##IDX).data.fd = fd##IDX;                                               \
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd##IDX, &(NAME##IDX));

#define EPOLLEVENTS 10

void showdata(const char *fname, int fd);

int main(int argc, char const *argv[]) {
  int fd1, fd2;
  int epoll_fd;
  int retval;
  int timeout;

  if (argc != 4) {
    fprintf(stderr, "usage:%s file file timeout\n", argv[0]);
    exit(1);
  }

  timeout = 1000 * atoi(argv[3]);

  epoll_fd = epoll_create1(0);
  if (epoll_fd == -1)
    oops("epoll_create1", 1);

  if ((fd1 = open(argv[1], O_RDONLY)) == -1)
    oops(argv[1], 2);
  if ((fd2 = open(argv[2], O_RDONLY)) == -1)
    oops(argv[2], 3);

  MAKE_EVENT(event, 1);
  MAKE_EVENT(event, 2);

  struct epoll_event events[EPOLLEVENTS];

  while (1) {
    retval = epoll_wait(epoll_fd, events, EPOLLEVENTS, timeout);
    if (retval == -1)
      oops("epoll", 4);
    if (retval > 0) {
      for (int i = 0; i < retval; i++) {
        int fd = events[i].data.fd;
        const char *device = fd == fd1 ? argv[1] : argv[2];
        if (events[0].events & EPOLLIN) {
          showdata(device, fd);
        }
      }
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
