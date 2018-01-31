/**
 *
 * man 7 pipe
 *
 * If  all  file  descriptors referring to the write end of a pipe have been
 * closed, then an attempt to read(2) from the pipe will see end-of-file
 *       (read(2) will return 0).  If all file descriptors referring to the read
 * end of a pipe have been closed, then a write(2) will  cause  a  SIGPIPE
 *       signal  to be generated for the calling process.  If the calling
 * process is ignoring this signal, then write(2) fails with the error EPIPE. An
 *       application that uses pipe(2) and fork(2) should use suitable close(2)
 * calls to close unnecessary duplicate file descriptors; this ensures that
 *       end-of-file and SIGPIPE/EPIPE are delivered when appropriate.
 *
 */
#include <arpa/inet.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int make_server_socket(uint16_t port, int backlog) {
  int listen_fd;
  struct sockaddr_in saddr;

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd == -1)
    return -1;

  memset((void *)&saddr, 0, sizeof(saddr));

  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listen_fd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    return -1;
  if (listen(listen_fd, backlog) != 0)
    return -1;
  return listen_fd;
}

int set_fd_nonblocking(int fd) {
  int flag = fcntl(fd, F_GETFL, 0);
  flag |= O_NONBLOCK;
  return 0;
}

int main(int argc, char const *argv[]) {
  if (argc < 2)
    return 1;
  int listen_fd = make_server_socket(atoi(argv[1]), 10);
  int client_fd;
  int status;

  while ((client_fd = accept(listen_fd, NULL, NULL)) != -1) {
    set_fd_nonblocking(client_fd);

    // 1st step
    status = send(client_fd, "hello", 5, 0);
    assert(status != -1);
    sleep(3);

    // 2nd step
    // send some data
    signal(SIGPIPE, SIG_IGN);
    while (1) {
      status = send(client_fd, "helloworld", 10, 0);
      if (status == -1)
        perror("send");
      else
        printf("no error %d\n", status);
      sleep(1);
    }

    close(client_fd); // without recv and close the connection.
  }
  return 0;
}
