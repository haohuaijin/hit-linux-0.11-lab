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

    int tmp, shmid;
    int position = 0;
    int *data;
    key_t key = 9999;

    shmid = shmget(key, (MAX_BUFFER+1)*sizeof(int), 0644 | IPC_CREAT);
    data = (int*)shmat(shmid, NULL, 0);

    while(1){
        sem_wait(full);
        sem_wait(mutex);

        position = (position % MAX_BUFFER);
        printf("%d: %d\n", getpid(), data[position]);
        fflush(stdout);
        if(data[position] == MAX_NUMBER){
            sem_post(mutex);
            sem_post(empty);
            break;
        }
        position++;
        
        sem_post(mutex);
        sem_post(empty);
    }
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    sem_close(full);
    sem_close(mutex);
    sem_close(empty);
    return 0;
}
