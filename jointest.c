#include "types.h"
#include "stat.h"
#include "user.h"

void func(void *arg1, void *arg2)
{
  int arg = (*(int*) arg1)++;
  printf(1, "%d", arg);
  exit();
}

int main(int argc, char *argv[])
{
  void *stack_bottom;
  
  int arg1 = 0;
  int n = 100;
  for(int i = 0; i < n ; i++){
    stack_bottom = malloc(4096);
    char *arg2 = "IN i = ";
    clone(&func, &arg1, arg2, stack_bottom);
  }
  for(int i = 0; i < n; i++)
    join(stack_bottom);
  
  printf(1, "All threads joined");
  printf(1, "END\n");
  printf(1, "%d", arg1);
  exit();
}
