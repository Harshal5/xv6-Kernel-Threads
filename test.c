#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"

char *echoargv[] = { "echo", "ALL", "TESTS", "PASSED", 0 };

void threadfunc(void *arg1, void *arg2) {
  exec("echo", echoargv);
  exit();
}
void threadfunc2(void *arg1, void *arg2) {
  printf(1, "In 2\n");
  exit();
}

int main(int argc, char *argv[])
{
  int i = 0;
  int pid = thread_create(threadfunc, &i, &i);
  // sleep(5);
  printf(1, " pid = %d\n", pid);
  thread_join();
  exit();
}
