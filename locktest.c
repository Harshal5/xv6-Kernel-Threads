#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"

tlock mylock;
int global = 0; 

void threadfunc(void *arg1, void *arg2) {
  for (int i = 0; i < 5; i++) {
    lock_acquire(&mylock);
    global++;
    lock_release(&mylock);
  }
  exit();
}

void threadfunc2(void *arg1, void *arg2) {
  for (int i = 1; i < 5; i++) {
    lock_acquire(&mylock);
    global--;
    lock_release(&mylock);
  }
  exit();
}

int main(int argc, char *argv[])
{
  lock_init(&mylock);
  int i = 0;
  thread_create(threadfunc, &i, &i);
  thread_create(threadfunc2, &i, &i);
  thread_join();
  thread_join();
  printf(1, "Global = %d\n", global);
  exit();
}
