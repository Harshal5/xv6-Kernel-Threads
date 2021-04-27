#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"

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

int main(){

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

    // printf(1, "%d %d\n", i, k);
    for(row = 0; row < i; row++){
        for(col = 0; col < k; col++){
            printf(1, "%d", result[row][col]);
        }
        printf(1, "\n");
    }

    exit();
}