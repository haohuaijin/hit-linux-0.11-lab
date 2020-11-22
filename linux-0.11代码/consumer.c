#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#define MAX_NUMBER 500
#define MAX_BUFFER 10

_syscall2(int, sem_open, const char*,name, unsigned int,value);
_syscall1(int, sem_wait, sem_t*, sem);
_syscall1(int, sem_post, sem_t*, sem);
_syscall1(int, sem_unlink, const char*, name);
_syscall3(int, shmget, int, key, size_t, size, int, shmflg);
_syscall3(int, shmat, int, shmid, const void *, shmaddr, int, shmflg);

int main(int argc, char * argv[]){
    sem_t* full = (sem_t*)sem_open("full", 0);
    sem_t* mutex = (sem_t*)sem_open("mutex", 1);
    sem_t* empty = (sem_t*)sem_open("empty", MAX_BUFFER);

    int tmp, shmid;
    int position = 0;
    int *data;
    int key = 9999;

    shmid = shmget(key, (MAX_BUFFER+1)*sizeof(int), 0);
    data = (int*)shmat(shmid, 0, 0);

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
    sem_unlink("full");
    sem_unlink("mutex");
    sem_unlink("empty");
    return 0;
}
