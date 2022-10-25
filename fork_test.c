#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include<sys/wait.h>

int main(){

    for (int i = 0; i < 3; i++){
        int id = fork();
        // if(id == 0){
        //     printf("i child = %d\n", i);
        // }
        // else printf("i Parent = %d\n", i);
        //printf("id = %d\n", id);
        if (id == 0){
            printf("I am a child, i = %d\n", i);
        }
    }

    for (int i = 0; i < 5; i++)
        wait(NULL);

    return 0; 
}