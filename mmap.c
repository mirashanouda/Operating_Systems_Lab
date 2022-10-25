/*
takes as input:
    The number of processes NPROC (assume it’s 2n-1)
    An integer T
Then computes the sum (⅀Ti=1 i) by dividing it equally (as much as possible) over NPROC processes
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/mman.h>



long long sumT(long long start, long long step){
    long long sum = 0;

    for(long long i = start + 1; i <= (start + step); i++){
        sum += i;
    }    
    return sum;
}

int main(){

    int in, T, nProc;
    printf("Number of processes: ");
    scanf("%d", &nProc);  //*************edit to take 2^n - 1;

    printf("The number for summation: "); 
    scanf("%d", &T); 
    
    long long step = T/nProc;

    int id;
    long long *totalsum = mmap(NULL, sizeof(long long), 
                        PROT_READ | PROT_WRITE, 
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);         

    for (int i = 0; i < step * nProc; i+= step){
        id = fork();
        if (id == 0){ //child process
            long long ps = sumT(i, step);
            *totalsum += ps; 
            i = step * nProc;
        }
    }

    for (long long i = 0; i < step * nProc; i += step){
    //for (int i = 0; i < nProc; i++){
        wait(NULL);
    }

    if (T%nProc != 0 && id != 0){ //only in the parent process 
        id = fork();
        if (id == 0){ //child process 
            long long start = step * nProc;
            long long end = T - start;
            //printf("start = %d, end = %d\n", start, end);
            long long ps = sumT(start, end);
            *totalsum += ps; 
        }
        wait(NULL);
    }
    if (id != 0)
        printf("\ntotal sum = %lld\n", *totalsum);
    return 0; 
}