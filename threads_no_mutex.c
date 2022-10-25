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

#define n 200
struct var {
    int num;
    int range;
};

//int NTHREAD; 
int sum = 0; 
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; 
pthread_t * thrd;

//pthread_t thrd[n];

void* sumT( void * x){
    
    //pthread_mutex_lock(&mtx);
    //struct var* arg = (struct var*) x;
    struct var arg = *(struct var*) x;
    int num =   arg.num;
    int range = arg.range;

    
    for(int i = num + 1; i <= (num + range); i++){
        
        //pthread_mutex_lock(&mtx);
        sum += i;
        //pthread_mutex_unlock(&mtx);
    }    
    
    //printf("Func num = %d -- Sum = %d\n", num, s);
    
    //pthread_mutex_unlock(&mtx);

    //return NULL;
    pthread_exit(NULL);
}


int main(){

    int NTHREAD; 
    int T; 
    
    printf("Number of threads: "); 
    scanf("%d", &NTHREAD); 

    printf("The number for summation: "); 
    scanf("%d", &T); 

    int err; 
    
    struct var* arg = malloc(sizeof(struct var) * (NTHREAD+1)); // ERROR here as 
    pthread_t * thrd = malloc(NTHREAD * sizeof(pthread_t)); 
    int t = 0; 
    int step = T/NTHREAD; 

    
    for (int i = 0; i < step * NTHREAD; i += step){
        arg[t].num = i;
        arg[t].range = step; 
        
        //printf("main num = %d, t = %d\n", arg->num, t);
        //printf("main range = %d\n", arg->range);

        err = pthread_create( &(thrd[t]), NULL, &sumT, (void *)&arg[t] ); 
        if(err != 0){
            printf("Can't create thread %d\n", t);
        }
        t++;
    }

    //last thread - handelling the case of unequal devision
    if (T%NTHREAD != 0){
        arg[NTHREAD].num = step * NTHREAD;
        arg[NTHREAD].range = T - arg[NTHREAD].num; 

        err = pthread_create( &(thrd[NTHREAD]), NULL, &sumT, (void *)&arg[NTHREAD]); 
        if(err != 0){
            printf("Can't create the last thread\n");
        }
    }

    for (int i = 0; i < NTHREAD; i++){
        pthread_join(thrd[i], NULL);
    }

    if (T%NTHREAD != 0)
         pthread_join(thrd[NTHREAD], NULL);
      

    pthread_mutex_destroy(&mtx);
    printf("Total Sum = %d --- PID = %d\n", sum , getpid());
    return 0;
}
