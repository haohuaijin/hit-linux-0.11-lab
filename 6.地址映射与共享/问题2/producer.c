#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>

#define MAX_NUMBER 500
#define MAX_BUFFER 10

int main(int argc, char * argv[]){
    //创建信号量
    sem_t* full = sem_open("/full", O_CREAT, 0666, 0);
    sem_t* mutex = sem_open("/mutex", O_CREAT, 0666, 1);
    sem_t* empty = sem_open("/empty", O_CREAT, 0666, MAX_BUFFER);

    int position, shmid;
    int *data;
    key_t key = 9999;

    shmid = shmget(key, (MAX_BUFFER+1)*sizeof(int), 0644 | IPC_CREAT);
    data = (int*)shmat(shmid, NULL, 0);

    for(int i=0; i < MAX_NUMBER+1; i++){
        sem_wait(empty);
        sem_wait(mutex);

        position = (i % MAX_BUFFER);
        data[position] = i;

        sem_post(mutex);
        sem_post(full);
    }
    shmdt(data);
    return 0;
}