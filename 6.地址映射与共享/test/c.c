#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/wait.h>

#define MAX_NUMBER 500
#define MAX_BUFFER 10

int main(int argc, char * argv[]){
    //创建信号量
    sem_t* full = sem_open("/full", O_CREAT, 0666, 0);
    sem_t* mutex = sem_open("/mutex", O_CREAT, 0666, 1);
    sem_t* empty = sem_open("/empty", O_CREAT, 0666, MAX_BUFFER);

    if(full == SEM_FAILED) printf("full error\n");
    if(mutex == SEM_FAILED) printf("mutex error\n");
    if(empty == SEM_FAILED) printf("emtpy error\n");

    int tmp=0, position=0;
    int fp = -1;
    while(1){
        sem_wait(full);
        sem_wait(mutex);

        fp = open("file", O_RDWR);
        position = (position % MAX_BUFFER);
        lseek(fp, position*sizeof(int), SEEK_SET);
        read(fp, &tmp, sizeof(int));
        printf("%d: %d\n", getpid(), tmp);
        fflush(stdout);
        if(tmp == MAX_NUMBER){
            sem_post(mutex);
            sem_post(empty);
            break;
        } 
        position++;
        close(fp);

        sem_post(mutex);
        sem_post(empty);
    }
    sem_close(full);
    sem_close(mutex);
    sem_close(empty);
    return 0;
}