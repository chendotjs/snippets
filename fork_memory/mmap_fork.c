/**
 * https://blog.csdn.net/maverick1990/article/details/48050975
 * mmap申请的空间在栈段和堆段之间，MAP_SHARED标志表示两个进程共享了mmap申请的内存，所以两个进程总共占用约1G内存.
 * MAP_PRIVATE会生成COW的映射
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#define K (1024)
#define M (1024 * K)

#define BUF_SIZE (1024 * M)

int main(int argc, char **argv) {
  char *p_map;

  /* 匿名映射,创建一块内存供父子进程通信 */
  p_map = (char *)mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  memset(p_map, 0, BUF_SIZE);

  if (fork() == 0) {
    while (1) {
      sleep(1);
      printf("child got a message: %s\n", p_map);
      sprintf(p_map, "%s", "hi, dad, this is son");
    }
    munmap(p_map, BUF_SIZE); //实际上，进程终止时，会自动解除映射。
    exit(0);
  }

  while (1) {
    sprintf(p_map, "%s", "hi, this is father");
    sleep(1);
    printf("parent got a message: %s\n", p_map);
  }

  return 0;
}
