#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis.h>

int main(int argc, char **argv) {
  unsigned int j;
  redisContext *c;
  redisReply *reply;
  const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";
  int port = (argc > 2) ? atoi(argv[2]) : 6379;

  struct timeval timeout = {1, 500000}; // 1.5 seconds
  c = redisConnectWithTimeout(hostname, port, timeout);
  if (c == NULL || c->err) {
    if (c) {
      printf("Connection error: %s\n", c->errstr);
      redisFree(c);
    } else {
      printf("Connection error: can't allocate redis c\n");
    }
    exit(1);
  }

  /* PING server */
  reply = redisCommand(c, "PING");
  printf("PING: %s\n", reply->str);
  freeReplyObject(reply);

  /* Set a key */
  reply = redisCommand(c, "SET %s %s", "foo", "hello world");
  printf("SET: %s\n", reply->str);
  freeReplyObject(reply);

  /* Get a key */
  reply = redisCommand(c, "GET %s", "foo");
  printf("GET %s: %s\n", "foo", reply->str);
  freeReplyObject(reply);

  redisAppendCommand(c, "SET foo lala");
  redisAppendCommand(c, "GET foo");
  redisGetReply(c, &reply); // reply for SET
  printf("SET foo: %s\n", reply->str);
  freeReplyObject(reply);
  redisGetReply(c, &reply); // reply for GET
  printf("GET foo: %s\n", reply->str);
  freeReplyObject(reply);

  reply = redisCommand(c, "SUBSCRIBE foo");
  printf("sub foo: %s\n", reply->str);
  freeReplyObject(reply);
  while (redisGetReply(c, (void **)&reply) == REDIS_OK) {
    if (reply == NULL) {
      return 1;
    }
    if (reply->type == REDIS_REPLY_ERROR) {
      printf("Error: return [REDIS_REPLY_ERROR]%s\n", reply->str);
    } else if (reply->type == REDIS_REPLY_STATUS) {
      if (strcmp(reply->str, "OK") != 0) {
        printf("Error: return [REDIS_REPLY_STATUS]%s\n", reply->str);
        freeReplyObject(reply);
        return 1;
      }
      printf("return [REDIS_REPLY_STATUS]: %d\n", reply->str);
    } else if (reply->type == REDIS_REPLY_INTEGER) {
      printf("return [REDIS_REPLY_INTEGER]: %lld\n", reply->integer);
    } else if (reply->type == REDIS_REPLY_STRING) {
      printf("return [REDIS_REPLY_STRING]: %s\n", reply->str);
    } else if (reply->type == REDIS_REPLY_ARRAY) {
      printf("return [REDIS_REPLY_ARRAY]:\n");
      for (int i = 0; i < reply->elements; i++) {
        printf("%d)%s\n", i + 1, reply->element[i]->str);
      }
      printf("\n");
    } else if (reply->type == REDIS_REPLY_NIL) {
      printf("return [REDIS_REPLY_NIL]: %s\n", reply->str);
    }
    freeReplyObject(reply);
  }

  /* Disconnects and frees the c */
  redisFree(c);

  return 0;
}
