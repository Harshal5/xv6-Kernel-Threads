#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void testfunc(void *arg1, void *arg2){
    printf(1, "Child: arg1 is %d\n", *(int*)arg1);
    printf(1, "Child: arg2 is %s\n", arg2);
    exit();
}
void clonetestfunc(void *arg1, void *arg2){
    int tgid; 
    // int clone_tid, join_tid;
    int a = 10;
    char *b = "string arg";
    void *stack;
    tgid = fork();
    if (!tgid){
        // child
        stack = malloc(4096);
        clone(testfunc, &a, b, stack, CLONE_THREAD | CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_PARENT);
        join(stack);
        free(stack);
        printf(1,"Child\n");
        exit();
    }

    wait();
    printf(1,"parent\n");
    exit();
}
int
main(int argc, char *argv[])
{
    int one1, two1; 
    void *stack11;
    one1 = 1;
    two1 = 1;
    stack11 = malloc(4096);
    clone(clonetestfunc, &one1, &two1, stack11, CLONE_THREAD | CLONE_VM | CLONE_PARENT);
    join(stack11);
    free(stack11);
    exit();
}
