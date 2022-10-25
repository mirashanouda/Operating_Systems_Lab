/*
Part 1: Write a C program that takes as input:
The number of processes NPROC (assume it’s 2n-1)
An integer T
Then computes the sum (⅀Ti=1 i) by dividing it equally (as much as possible) over NPROC processes [1.5 Points].


Part 2: Write the same program in Part 1 but with NTHREAD threads. Run it multiple times without using mutexes and then use mutexes 
and run it again. Explain outputs [2 Points].
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct var {
    int num;
    int range;
};

//int NTHREAD; 
long long sum = 0; 
pthread_mutex_t mtx; 

void* sumT( void * x){
    struct var* arg = ((struct var*) x);
    int num =   arg->num;
    int range = arg->range;

    
    for(int i = num + 1; i <= (num + range); i++){
        pthread_mutex_lock(&mtx);
        sum += i;
        pthread_mutex_unlock(&mtx);
    }    
    pthread_exit(NULL);
}


int main(int argc, char** argv){

    if (argc != 3){
        printf("Error! Missing the number of threads and/or the number to sum\n" );
        return -1;
    } 

    double time_spent = 0.0;
    clock_t begin = clock();

    int NTHREAD, T;
    NTHREAD = atoi(argv[1]);
    T = atoi(argv[2]);

    int err; 
    
    struct var* arg = malloc(sizeof(struct var) * (NTHREAD+1)); 
    
    pthread_t * thrd = malloc((NTHREAD+1) * sizeof(pthread_t)); //thread identifier 
    
    int t = 0; 
    int step = T/NTHREAD; 


    for (int i = 0; i < step * NTHREAD; i += step){
        arg[t].num = i;
        arg[t].range = step; 

        err = pthread_create( &(thrd[t]), NULL, &sumT, (void *)&arg[t] ); //returns 0 or error num
        /**
         * thrd: location where created thread ID is stored  
         * threads attibutes - NULL: default - thread information
         * &sumT: pointer to the function
         * pointer to the arguments of the function
         */
        
        if(err != 0){
            printf("Can't create thread %d\n", t);
        }
        t++; //threads counter
    }

    //last thread - handelling the case of unequal devision
    if (T%NTHREAD != 0){
        arg[NTHREAD].num = step * NTHREAD;
        arg[NTHREAD].range = T - arg[NTHREAD].num; 

        err = pthread_create( &(thrd[NTHREAD]), NULL, &sumT, (void *)&arg[NTHREAD]); 
        if(err != 0){
            printf("Can't create the last thread\n");
            return 0;
        }
    }

    for (int i = 0; i < NTHREAD; i++){
        err = pthread_join(thrd[i], NULL);
        if (err != 0)
            printf("Error! Failed to retrun from the thread");
        /**
         * thread ID
         * pointer to store the exit status
         * retruns: 0 or error num
         */
    }

    if (T%NTHREAD != 0){
        err = pthread_join(thrd[NTHREAD], NULL);
        if (err != 0)
            printf("Error! Failed to retrun from the thread");
    }
        
    printf("Total Sum = %lld --- PID = %d\n", sum, getpid());

    free(arg);
    free(thrd);
    clock_t end = clock();

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    printf("The elapsed time is %f seconds\n", time_spent);
    return 0;
}