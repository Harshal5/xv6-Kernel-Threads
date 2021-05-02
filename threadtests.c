#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"

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

// test for userland thread library
int userlandFuncTest(void){
    printf(1, "UserlandFuncTest starting\n");
    int i = 0;
    int create_pid, join_pid;
    create_pid = thread_create(testfunc, &i, &i);
    join_pid = thread_join();
    if(create_pid == join_pid){
        printf(1, "UserlandFuncTest passed\n\n");
    } else{
        printf(1, "UserlandFuncTest failed\n\n");
    }
    return 0;
}

// Matrix multiplication code
#define MAX_SIZE 100000

int count = 0, i, j, k;

long int *matrix1[MAX_SIZE];
long int *matrix2[MAX_SIZE];

long int *result[MAX_SIZE];

void calculate(){
    int divider = count++;
    for(int a = divider * i/3; a < (divider+1) * i/3; a++){
        for (int b = 0; b < k; b++)  
            for (int c = 0; c < j; c++)  
                result[a][b] += matrix1[a][c] * matrix2[c][b];
    }
    exit();
}

int matmulTest(){
    printf(1, "MatrixMultplicationTest starting\n");
    int row, col;
    
    i = 3;
    j = 2;
    int value = 0;

    for(row = 0; row < i; row++){
        matrix1[row] = (long int *) malloc(j * sizeof(long int));
    }

    for(row = 0; row < i; row++){
        for(col = 0; col < j; col++){
            matrix1[row][col] = value++;
        }
    }
    
    int isMultiplicable;
    isMultiplicable = 2;
    k = 1;

    if(isMultiplicable != j){
        printf(1, "Check Dimensions for multiplication\n");
        return 1;
    }

    for(row = 0; row < j; row++){
        matrix2[row] = (long int *) malloc(k * sizeof(long int));
    }

    for(row = 0; row < j; row++){
        for(col = 0; col < k; col++){
            matrix2[row][col] = value++;
        }
    }

    for(row = 0; row < i; row++){
        result[row] = (long int *) malloc(k * sizeof(long int));
    }    

    int thread_id = 1;

    for(int thread_num = 0; thread_num < 3; thread_num++){
        thread_create(calculate, &thread_id, &thread_id);
    }

    for(int thread_num = 0; thread_num < 3; thread_num++){
        thread_join();
    }

    printf(1, "Output: \n");
    for(row = 0; row < i; row++){
        for(col = 0; col < k; col++){
            printf(1, "%d", result[row][col]);
        }
        printf(1, "\n");
    }
        printf(1, "MatrixMultplicationTest passed\n\n");

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
    clone(exectestfunc, &one1, &two1, stack1, CLONE_VM | CLONE_PARENT | CLONE_THREAD );
    join(stack1);
    free(stack1);
    printf(1, "ExecInCloneTest passed\n\n");
    return 0;
}

void func(void *arg1, void *arg2)
{
    printf(1, "Child1 pid = %d\n", getpid());
    printf(1, "Child1 tid = %d\n", gettid());
    if(gettid() > 2 && getpid() > 2){
        printf(1, "getTidTest passed\n\n");
    } else{
        printf(1, "getTidTest failed\n\n");
    }
    exit();
}

int getTidTest(void){
    printf(1, "getTidTest passed\n");
    void *stack = malloc(4096);
    int arg1 = 0;
    char *arg2 = "Hello";
    printf(1, "Parent tid = %d\n", gettid());
    printf(1, "Parent pid = %d\n", getpid());
    clone(&func, &arg1, arg2, stack, CLONE_THREAD);
    join(stack);
    return 0;
}

tlock mylock;
int global = 0; 

void lockfunc1(void *arg1, void *arg2) {
    for (int i = 0; i < 10; i++) {
      lock_acquire(&mylock);
      global++;
      lock_release(&mylock);
    }
    exit();
}

void lockfunc2(void *arg1, void *arg2) {
    for (int i = 1; i < 10; i++) {
      lock_acquire(&mylock);
      global--;
      lock_release(&mylock);
    }
    exit();
}

int locksTest(void){
    printf(1, "LocksTest starting\n");
    lock_init(&mylock);
    int i = 0;
    thread_create(lockfunc1, &i, &i);
    thread_create(lockfunc2, &i, &i);
    thread_join();
    thread_join();
    if(global == 1){
        printf(1, "LocksTest passed\n\n");
    } else{
        printf(1, "LocksTest failed\n\n");
    }
    return 0;
}


int tgkillTest(void){
    printf(1, "tgKillTest started\n");
    void *stack = malloc(4096);
    int arg1 = 0;
    char *arg2 = "Hello";
    int cloned_tid = clone(&testfunc, &arg1, arg2, stack, CLONE_THREAD);
    int killed = tgkill(getpid(), cloned_tid);
    if(killed == 0){
      printf(1, "tgKillTest passed\n\n");
    } else{
      printf(1, "tgKillTest failed\n\n");
    }

    return 0;
}

void ffunc(void *arg1, void *arg2) {
    (*(int*) arg1)++;
    exit();
}

int maxThreadsTest(void){
    printf(1, "MaximumThreadTest starting\n");
    void *stack[100];
    int arg1 = 0, total = 1;
    int n = 100;
    for(int i = 0; i < n ; i++){
        stack[i] = malloc(4096);
        if(clone(&ffunc, &arg1, &arg1, stack[i], CLONE_THREAD | CLONE_VM) < 0){
            break;
        } else{
            total++;
        }
    }
    for(int i = 0; i < total; i++){
        join(stack[i]);
        free(stack[i]);
    }

    printf(1, "Maximum Thread Limit reached %d\n\n", total);
    return 0;   
}

int limitedThreadsTest(void){
    printf(1, "LimitedThreadsTest starting\n");
    void *stack[50];
    int arg1 = 0, total = 1;
    int n = 50;
    for(int i = 0; i < n ; i++){
        stack[i] = malloc(4096);
        if(clone(&ffunc, &arg1, &arg1, stack[i], CLONE_THREAD | CLONE_VM) < 0){
            printf(1, "Limited Threads Test Failed\n\n");         
        } else{
            total++;
        }
    }
    for(int i = 0; i < n; i++){
        join(stack[i]);
        free(stack[i]);
    }
    printf(1, "Limited Threads Test Passed %d\n\n", total);
    return 0;
}

void fstestfunc(void *a, void *b){
    mkdir("test");
    chdir("test");
    exit();
}

int cloneFSTest(void)
{
    printf(1, "clone fs starting\n");
    void *stack = malloc(4096);
    int arg1 = 0; 
    clone(fstestfunc, &arg1, &arg1, stack, CLONE_THREAD | CLONE_FS);
    join(stack);
    
    if (chdir("../") < 0){
        printf(1, "clone fs test FAILED\n");
        return 0;
    }
    if (unlink("test") < 0){
        printf(1, "unlink failed\n");
        return 0;
    }
    free(stack);
    printf(1, "CloneFsTest passed\n");
    return 0;
}

void filesfunc(void *arg1, void *arg2) {
    write(*(int*)arg1, "Hello\n", strlen("Hello\n"));
    exit();
}

int cloneFilesTest(void){
    printf(1, "CloneFilesTest started\n");   
    void *stack = malloc(4096);
    int fd = open("flags.txt", O_RDONLY|O_WRONLY|O_CREATE);
    clone(filesfunc, &fd, &fd, stack, CLONE_THREAD | CLONE_FILES);
    join(stack);
    write(fd, "in  MAIN\n", strlen("in MAIN\n"));
    free(stack);
    printf(1, "CloneFilesTest Passed\n\n");   
    return 0;
}   
    
int main(int argc, char *argv[]){
    cloneTest();
    cloneInForkTest();
    cloneFSTest();
    cloneFilesTest();
    mutlipleCloneTest();
    forkInCloneTest();
    userlandFuncTest();
    getTidTest();
    locksTest();
    tgkillTest();
    maxThreadsTest();
    limitedThreadsTest();
    matmulTest();
    printf(1, "All THREADTESTS COMPLETED\n");
    exit();
}