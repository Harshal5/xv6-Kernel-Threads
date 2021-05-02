#include "types.h"
#include "stat.h"
#include "user.h"


int main(int argc, char *argv[])
{
  int arg1 = 0;
  char *arg2 = "Hello";
  void *stack = malloc(4096);
  clone(&func, &arg1, arg2, stack, CLONE_THREAD);
  printf(1,"main\n");
  join(stack);
  printf(1, "Parent pid = %d\n", getpid());
  printf(1, "Parent tid = %d\n", gettid());
  printf(1, "All threads joined");
  printf(1, "END\n");
  exit();
}