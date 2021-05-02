#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void testfunc(void *arg1, void *arg2){
    printf(1, "Printing by threads\n");
    exit();
}

// test for one thread creation
int cloneTest(void){
    printf(1, "CloneTest starting\n");
    int clone_tid, join_tid;
    void *stack = malloc(4096);
    int arg1 = 10;
    char *arg2 = "string arg";
    clone_tid = clone(testfunc, &arg1, arg2, stack, CLONE_THREAD | CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_PARENT);
    join_tid = join(stack);
    free(stack);
    if(clone_tid == join_tid){
        printf(1, "CloneTest passed\n\n");
    } else{
        printf(1, "CloneTest failed\n\n");
    }
    return 0;
}

// test for creating thread in fork
int cloneInForkTest(void){
    printf(1, "CloneInForkTest starting\n");
    int tgid, clone_tid, join_tid;
    int arg1 = 10;
    char *arg2 = "string arg";
    void *stack;
    tgid = fork();
    if (!tgid){
        // child
        stack = malloc(4096);
        clone_tid = clone(testfunc, &arg1, arg2, stack, CLONE_THREAD | CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_PARENT);
        join_tid = join(stack);
        free(stack);
        if(clone_tid == join_tid){
            printf(1, "CloneInForkTest passed\n\n");
        } else{
            printf(1, "CloneInForkTest failed\n\n");
        }    
        exit();
    }
    wait();
    return 0;
}

// test for creating multiple threads 
int mutlipleCloneTest(void){
    printf(1, "MutlipleCloneTest starting\n");
    void *stack1 = malloc(4096);
    void *stack2 = malloc(4096);
    int arg1 = 10;
    char *arg2 = "string arg";
    clone(testfunc, &arg1, arg2, stack1, CLONE_THREAD | CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_PARENT);
    join(stack1);
    clone(testfunc, &arg1, arg2, stack2, CLONE_THREAD | CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_PARENT);
    join(stack2);
    free(stack1);
    free(stack2);
    printf(1, "MutlipleCloneTest passed\n\n");
    return 0;
}

void clonetestfunc(void *arg1, void *arg2){
    int tgid; 
    tgid = fork();
    if (!tgid){
        printf(1,"In Forked Child\n");
        exit();
    }
    wait();
    printf(1,"In Forked Parent\n");
    exit();
}

// test for using clone in fork
int forkInCloneTest(){
    printf(1, "ForkInCloneTest starting\n");
    int one1, two1; 
    void *stack1;
    one1 = 1;
    two1 = 1;
    stack1 = malloc(4096);
    clone(clonetestfunc, &one1, &two1, stack1, CLONE_VM | CLONE_PARENT);
    join(stack1);
    free(stack1);
    printf(1, "ForkInCloneTest passed\n\n");
    return 0;
} 

void exectestfunc(void *arg1, void *arg2) {
    char *echoargv[] = { "echo", "From echo:", "EchoinCloneTest","passed", 0 };
    exec("echo", echoargv);
    exit();
}

int execInCloneTest(void){
    printf(1, "ExecInCloneTest starting\n");
    int one1, two1; 
    void *stack1;
    one1 = 1;
    two1 = 1;
    stack1 = malloc(4096);
    clone(exectestfunc, &one1, &two1, stack1, CLONE_VM | CLONE_PARENT | CLONE_THREAD);
    join(stack1);
    free(stack1);
    printf(1, "ExecInCloneTest passed\n\n");
    return 0;
}

int main(int argc, char *argv[]){
    printf(1, "Starting with tests for all possible ways for calling clone and join\n\n");
    cloneTest();
    cloneInForkTest();
    mutlipleCloneTest();
    forkInCloneTest();
    execInCloneTest();
    exit();
}