#include "types.h"
#include "stat.h"
#include "user.h"

void func(void *arg1, void *arg2)
{
  printf(1, "Child pid = %d\n", getpid());
  printf(1, "Child tid = %d\n", gettid());
  exit();
}

int main(int argc, char *argv[])
{
  void *stack_bottom = malloc(4096);
  int arg1 = 0;
  char *arg2 = "Hello";
  clone(&func, &arg1, arg2, stack_bottom, CLONE_THREAD);
  // sleep(100);
  join(stack_bottom);
  printf(1, "Parent pid = %d\n", getpid());
  printf(1, "Parent tid = %d\n", gettid());
  printf(1, "All threads joined");
  printf(1, "END\n");
  exit();
}
