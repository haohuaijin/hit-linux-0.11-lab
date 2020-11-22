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

    int position, shmid;
    int *data;
    int key = 9999;
    int i;

    shmid = shmget(key, (MAX_BUFFER+1)*sizeof(int), 0);
    data = (int*)shmat(shmid, 0, 0);

    for(i=0; i < MAX_NUMBER+1; i++){
        sem_wait(empty);
        sem_wait(mutex);

        position = (i % MAX_BUFFER);
        data[position] = i;

        sem_post(mutex);
        sem_post(full);
    }
    return 0;
}