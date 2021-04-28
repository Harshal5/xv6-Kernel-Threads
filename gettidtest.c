#include "types.h"
#include "stat.h"
#include "user.h"

void func1(void *arg1, void *arg2)
{
  sleep(10);
  printf(1, "Child2 pid = %d\n", getpid());
  printf(1, "Child2 tid = %d\n", gettid());
  exit();
}

void func(void *arg1, void *arg2)
{
  printf(1, "Child1 pid = %d\n", getpid());
  printf(1, "Child1 tid = %d\n", gettid());
  void *stack = malloc(4096);
  clone(&func1, &arg1, arg2, stack, CLONE_THREAD);
  // sleep(5);
  // int killed = kill(send_tid);
  // printf(1, "Killed: %d\n", killed);
  // join(stack);
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
  join(stack_bottom);
  printf(1, "Parent pid = %d\n", getpid());
  printf(1, "Parent tid = %d\n", gettid());
  printf(1, "All threads joined");
  printf(1, "END\n");
  exit();
}
