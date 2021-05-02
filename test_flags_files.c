#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"

void threadfunc(void *arg1, void *arg2) {
  write(*(int*)arg1, "Hello\n", strlen("Hello\n"));
  exit();
}

int main(int argc, char *argv[])
{
  char s[5];
  char *arg2 = "Hello";
  int fd = open("flags.txt", O_RDONLY|O_WRONLY|O_CREATE);
  thread_create(threadfunc, &fd, arg2);
  thread_join();
  write(fd, "in MAIN\n", strlen("in MAIN\n"));
  printf(1, "read in main %s\n", s);
  fd = open("flags.txt", O_RDONLY|O_WRONLY|O_CREATE);
  read(fd, s, sizeof(s));
  close(fd);
  printf(1, "END\n");
  exit();
}
