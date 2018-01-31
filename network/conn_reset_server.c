/**
 * This can happen if the other side crashes and then comes back up or if it
 * calls close() on the socket while there is data from you in transit, and is
 * an indication to you that some of the data that you previously sent may not
 * have been received.
 *
 *       _ 0              other side normally close
 * recv |
 *      |- EAGAIN         no data, connection alive
 *      |_ ECONNRESET     other side send rst packet
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
  char recv_buf[BUFSIZ];

  while ((client_fd = accept(listen_fd, NULL, NULL)) != -1) {
    set_fd_nonblocking(client_fd);

    // 1st step
    // send some data which will not be received by client
    status = send(client_fd, "hello", 5, 0);
    assert(status != -1);
    sleep(3);

    // 2nd step
    // recv some data
    status = recv(client_fd, recv_buf, sizeof(recv_buf), 0);
    if (status == -1)
      perror("recv");
    else
      printf("no error %d\n", status);
    close(client_fd); // without recv and close the connection.
  }
  return 0;
}
