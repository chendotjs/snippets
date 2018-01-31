#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int connect_to_server(uint16_t port) {
  static const char *host = "127.0.0.1";
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  assert(fd != -1);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  int status = inet_pton(AF_INET, host, &addr.sin_addr);
  assert(status != -1);

  status = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
  assert(status != -1);

  return fd;
}

int main(int argc, char const *argv[]) {
  if (argc < 2)
    return 1;
  int fd;

  fd = connect_to_server(atoi(argv[1]));
  assert(fd != -1);
  printf("connect_to_server\n");

  close(fd);
  printf("exit\n");
  
  return 0;
}
