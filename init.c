// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char *argv[] = { "sh", 0 };

int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){    // open the file called console   // and returns 0 fd
    mknod("console", 1, 1);           // if it does not exist create the file
    open("console", O_RDWR);
  }
  dup(0);  // stdout    // duplicating it to console  
  dup(0);  // stderr    // duplicating it to console

  for(;;){
    printf(1, "init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      exec("sh", argv);   // execute the shell (the prompt which we see and executes the command)
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)    // waiting for zombie processes or cleaning up the zombie processes
      printf(1, "zombie!\n");                   // a zommbie process is a process which is over but the parent has not called a wait on it
  }
}
