#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
//int parent_id = get_id();




int getSum(int low, int high)
{
    int sum=0;
    for (int i = low; i <=high; i++)
        sum += i;
    return sum;
}

int main()
{
    int NPROC = 2; // 2^2-1 = 4-1 =3
    int value =20;
    int start[NPROC]; //each process has a start and an end
    int end[NPROC];
    int d= value/NPROC;
    int fd[2*NPROC];
    int totalsum =0;

//filling the start and end arrays
    for(int i=0, j= 0; i< NPROC; i++)
    {
        start[i]= j+1; 
        printf("start: %d\n", start[i]);
        end[i]= j+d;
        printf("end: %d\n", end[i]);
        j += d;
    }
    for (int i =0 ; i< NPROC; i++)
    {
        ///int fd[2];
        pipe(fd);
    }

    for(int i=0, j=0; i < NPROC; i++) // loop will run n times (n=5)
    {
        
        pid_t pid= fork();
        if(pid == 0)
        {
            int sum = getSum(start[i],end[i]);
            printf("childsum: %d\n", sum);
            close(fd[j]);
            //printf("write to j = %d\n", j);
            write(fd[j+1], &sum, sizeof(sum));
            printf("write to j = %d\n", j+1);
            //close(fd[j+1]); 
            j +=2;
            exit(0);
        }
    }
    for(int i=0;i<NPROC;i++) // loop will run n times (n=5)
        wait(NULL);
    
    for(int i=0, j=0;i<NPROC;i++) // loop will run n times (n=5)
    {
        j = i;
        close(fd[j+1]);
        int sum3;
        read(fd[j] , &sum3, sizeof(sum3));
        printf("read from j = %d\n", j);
        printf("sum3: %d\n",sum3);
        //close(fd[j]); 
        totalsum += sum3;
        printf("Total sum = %d \n",totalsum);
        j+=2;
    }
   
    return 0;
}

// #include<stdio.h>
// int main()
// {
//     for(int i=0;i<5;i++) // loop will run n times (n=5)
//     {
//         if(fork() == 0)
//         {
//             printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid());
//             exit(0);
//         }
//     }
//     for(int i=0;i<5;i++) // loop will run n times (n=5)
//     wait(NULL);



    // pid_t pid= fork();
    // if(pid ==0)
    // {
    //     int sum = getSum(1,10);
    //     printf("childsum: %d\n", sum);
    //     close(fd[0]);
    //     write(fd[1],&sum, sizeof(sum));
    //     close(fd[1]);
    // }
    // else{
    //     int sum2 = getSum(11,20);
    //     wait(NULL);
    //     int sum3;
    //     close(fd[1]);
    //     read(fd[0], &sum3,sizeof(sum3));
    //     close(fd[0]);
    //     int totalsum = sum2+ sum3;
    //      printf("Total sum = %d \n",totalsum);
    // }