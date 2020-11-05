#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NR_PROC 3
#define MAX_NUMBER 50
#define MAX_BUFFER 10

_syscall2(int, sem_open, const char*,name, unsigned int,value);
_syscall1(int, sem_wait, sem_t*, sem);
_syscall1(int, sem_post, sem_t*, sem);
_syscall1(int, sem_unlink, const char*, name);

int main(int argc, char * argv[]){
    sem_t* full = (sem_t*)sem_open("full", 0);
    sem_t* mutex = (sem_t*)sem_open("mutex", 1);
    sem_t* empty = (sem_t*)sem_open("empty", MAX_BUFFER);

    pid_t pid[NR_PROC];
    int tmp=0, position=0;
    int i;
    int fp = open("file", O_CREAT | O_RDWR | O_TRUNC, 0666);

    for(i = 0; i < NR_PROC; i++) {
        pid_t cur_pid = fork();
        if(cur_pid == 0) { 
            while(1){
                sem_wait(full);
                sem_wait(mutex);
                

                /*读取上一次读取出来的数字*/
                lseek(fp, 0, SEEK_SET);
                read(fp, (void*)&tmp, sizeof(int));
                if(tmp == MAX_NUMBER){
                    
                    sem_post(mutex);
                    sem_post(empty);
                    break;
                }

                /* 读取本次的数据 */
                position = ((tmp+1) % MAX_BUFFER) + 1;
                lseek(fp, position*sizeof(int), SEEK_SET);
                read(fp, (void*)&tmp, sizeof(int));
                printf("%d: %d\n", getpid(), tmp);
                fflush(stdout);

                /*写入本次读取的数据*/
                lseek(fp, 0, SEEK_SET);
                write(fp, (void*)&tmp, sizeof(int));

                
                sem_post(mutex);
                sem_post(empty);
            }
            /*printf("I'm exit, pid: %d\n", getpid());*/
            return 0;
        }
    }
    tmp = -1; position = 0;
    lseek(fp, 0, SEEK_SET);
    write(fp, (void*)&tmp, sizeof(int)); /*写入标志位*/
    /*这里在循环中加上NR_PROC是为了让子进程能够退出循环。*/
    for(i=0; i < MAX_NUMBER+NR_PROC+1; i++){ 
        sem_wait(empty);
        sem_wait(mutex);
        

        position = (i % MAX_BUFFER) + 1;
        lseek(fp, position*sizeof(int), SEEK_SET);
        write(fp, (void*)&i, sizeof(int));

        sem_post(mutex);
        sem_post(full);
        
    }

    for(i = 0; i < NR_PROC; i++){
        wait(&pid[i]);
    }
    close(fp);
    sem_unlink("empty");
    sem_unlink("mutex");
    sem_unlink("full");
    /*printf("the father exit\n");*/
    return 0;
}


