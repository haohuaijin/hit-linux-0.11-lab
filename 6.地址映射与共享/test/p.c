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
    
    int tmp=-1, position=0;
    int fp = open("file", O_CREAT | O_RDWR | O_TRUNC, 0666);
    close(fp);
    //这里在循环中加上NR_PROC是为了让子进程能够退出循环。
    for(int i=0; i < MAX_NUMBER+10; i++){ //生产者
        sem_wait(empty);
        sem_wait(mutex);

        fp = open("file", O_RDWR);
        position = (i % MAX_BUFFER);
        lseek(fp, position*sizeof(int), SEEK_SET);
        write(fp, (void*)&i, sizeof(int));
        close(fp);

        sem_post(mutex);
        sem_post(full);
    }
    return 0;
}