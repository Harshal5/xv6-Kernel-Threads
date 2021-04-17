#include "types.h"
#include "stat.h"
#include "user.h"

void func(void *arg1, void *arg2)
{
  int pid = getpid();
  printf(1, "Child: pid is %d\n", pid);
  printf(1, "Child: arg1 is %d\n", *(int*)arg1);
  printf(1, "Child: arg2 is %s\n", arg2);
  exit();
}

int main(int argc, char *argv[])
{
  int parent_pid, child_pid;
  void *stack_bottom = malloc(4096);
  int arg1 = 10;
  char *arg2 = "Harshal";
  printf(1, "Parent : arg1 is %d\n", arg1);
  printf(1, "Parent : arg2 is %s\n", arg2);

  parent_pid = getpid();
  printf(1, "Parent: pid is %d\n", parent_pid);

  child_pid = clone(&func, &arg1, arg2, stack_bottom);
  // sleep while the cloned process runs
  // we do this so that we can run this test without using join()
  sleep(10);
  printf(1, "Parent: pid of cloned thread is %d\n", child_pid);

  printf(1, "END\n");
  exit();

}