#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <pthread.h>



long long sumT(int start, int step){
    long long sum = 0;

    for(int i = start + 1; i <= (start + step); i++){
        sum += i;
    }    
    return sum;
}

int main(int argc, char** argv){

    if (argc != 3){
        printf("Error! Missing the number of threads and/or the number to sum\n" );
        return -1;
    } 
    int in, T, nProc;
    
    double time_spent = 0.0;
    clock_t begin = clock();

    nProc = atoi(argv[1]);
    T = atoi(argv[2]);
    

    int step = T/nProc;
    int id;
    long long totalsum = 0;

    for (int i = 0; i < step * nProc; i+= step){
        int fd[2];
        if(pipe(fd) == -1){ //0 fro sucess
            printf("Error creating a pip at i = %d\n", i);
            return 1;
        }
        id = fork();
        if (id == 0){ //child process
            long long ps = sumT(i, step);
            close(fd[0]); //
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

    for (int i = 0; i < step * nProc; i += step){
        wait(NULL); //pointer to exit status -- NULL:no need 
    }

//while wait != -1

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
            int ps = sumT(start, end);
            close(fd[0]);
            write(fd[1], &ps, sizeof(ps));
            close(fd[1]);
        }
        else {
            int sumFromChild;
            close(fd[1]); //close writing
            read(fd[0], &sumFromChild, sizeof(sumFromChild));
            close(fd[0]);
            totalsum += sumFromChild;
        }
    }

    if (id != 0)
    {
        printf("Total sum = %lld\n", totalsum);
        clock_t end = clock();

        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

        printf("The elapsed time is %f seconds\n", time_spent);
    }   
    return 0; 
}