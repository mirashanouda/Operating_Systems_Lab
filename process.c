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



long long sumT(int start, int step){
    long long sum = 0;

    for(int i = start + 1; i <= (start + step); i++){
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
    
    int step = T/nProc;
    
    //printf("step = %d\n", step);
    //int fd[2*nProc];

    //int pipes = 2* (nProc^2 - 1);
    //int fd[pipes];
    int id;
    long long totalsum = 0;

    for (int i = 0; i < step * nProc; i+= step){
        int fd[2];
        if(pipe(fd) == -1){
            printf("Error creating a pip at i = %d\n", i);
            return 1;
        }
        id = fork();
        if (id == 0){ //child process
            long long ps = sumT(i, step);
            close(fd[0]); //close reading
            write(fd[1], &ps, sizeof(ps));
            close(fd[1]);
            i = step * nProc; //terminating the child 
            //kill(getpid(), 0);
        }
        else {
            long long sumFromChild;
            close(fd[1]); //close writing
            read(fd[0], &sumFromChild, sizeof(sumFromChild));
            close(fd[0]);
            //printf("Partial sum = %d\n", sumFromChild);
            totalsum += sumFromChild;
            //printf("\ntotal sum = %d\n", totalsum);
        }
    }

//while wait != -1
    for (int i = 0; i < step * nProc; i += step){
    //for (int i = 0; i < nProc; i++){
        wait(NULL); // status is not needed 
    }



    if (T%nProc != 0 && id != 0){ //only in the parent process 
        int fd[2];
        if(pipe(fd) == -1){
            printf("Error creating the last pip\n");
            return 1;
        }
        id = fork();
        if (id == 0){ //child process 
            int start = step * nProc;
            int end = T - start;
            //printf("start = %d, end = %d\n", start, end);
            int ps = sumT(start, end);
            close(fd[0]); //close reading
            write(fd[1], &ps, sizeof(ps));
            close(fd[1]);
        }
        else {
            int sumFromChild;
            close(fd[1]); //close writing
            read(fd[0], &sumFromChild, sizeof(sumFromChild));
            close(fd[0]);
            //printf("Partial sum = %d\n", sumFromChild);
            totalsum += sumFromChild;
            //printf("\ntotal sum = %d\n", totalsum);
        }
    }

    if (id != 0)
        printf("\ntotal sum = %lld\n", totalsum);
    return 0; 
}