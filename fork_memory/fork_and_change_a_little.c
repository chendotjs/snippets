/**
 * 在堆上申请1G的空间，事实证明父子进程中，申请空间的虚拟内存地址是相同的（COW千万不要以为这就是一块内存）。如果子进程不调用memset，那么父子进程会共享
 * 同一片物理内存，两个进程占用的空间为1G。子进程调用memset之后，两个进程占用的空间为1G，这是因为只有256个物理页框变化了。
 */

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
    while (1) {
      printf("father: %d %p\n", getpid(), memo);
      write(1, memo, 10);
      sleep(1);
    }
  } else if (r == 0) {
    memset(memo, 'd', 1 * M); // 256 pages chages if a page = 4k 
    while (1) {
      printf("child: %d %p\n", getpid(), memo);
      write(1, memo, 10);
      sleep(1);
    }
  } else {
    perror("fork failed");
    exit(1);
  }

  return 0;
}
