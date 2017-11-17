/* similar to one-reader-one-writer */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define oops(m, x)                                                             \
  do {                                                                         \
    perror(m);                                                                 \
    exit(x);                                                                   \
  } while (0)

struct thread_info {
  pthread_t thread_id;
  char *thread_name;
};

#ifdef MUTEX
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER,
                mutex2 = PTHREAD_MUTEX_INITIALIZER;
#endif

void *foo_say_hello(void *arg) {
  struct thread_info *ptinfo = arg;
  while (1) {
#ifdef MUTEX
    pthread_mutex_lock(&mutex1);
#endif
    printf("[%ld]: %s\n", ptinfo->thread_id, ptinfo->thread_name);
    sleep(1);
#ifdef MUTEX
    pthread_mutex_unlock(&mutex2);
#endif
  }
  return NULL;
}

void *bar_say_hello(void *arg) {
  struct thread_info *ptinfo = arg;
  while (1) {
#ifdef MUTEX
    pthread_mutex_lock(&mutex2);
#endif
    printf("[%ld]: %s\n", ptinfo->thread_id, ptinfo->thread_name);
    sleep(1);
#ifdef MUTEX
    pthread_mutex_unlock(&mutex1);
#endif
  }
  return NULL;
}

int main(int argc, char const *argv[]) {
  int i;
  int ret;
  struct thread_info tinfo[2] = {{0, "foo"}, {0, "bar"}};

#ifdef MUTEX
  // let bar say hello first
  pthread_mutex_lock(&mutex1);
#endif

  for (i = 0; i < 2; i++) {
    void *(*say_hello)(void *) = i == 0 ? foo_say_hello : bar_say_hello;
    ret = pthread_create(&tinfo[i].thread_id, NULL, say_hello,
                         (void *)(&tinfo[i]));
    if (ret != 0)
      oops("pthread_create", 1);
  }
  pthread_exit(NULL);
  return 0;
}
