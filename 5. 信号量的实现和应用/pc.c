#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>

#define NR_PROC 5
#define MAX_NUMBER 500
#define MAX_BUFFER 10
/*
 * 由于没有函数可以直接对文件进行删除，所以我们要想对文件读取后删除，这里有两种方法
 * 第一种：将文件全部读取出来，然后关闭文件，重新打开时利用open()将文件清0，然后将
 * 读取的文件中第一个打印出来，剩下的全部在重新写入到文件里。不过这样太浪费时间来。
 * 第二种：在文件的第一个位置设置一个标志，里面存放的是上一个消费者从里面读出来的数据，
 * 这样本次就可以通过上一个消费者读取的数据，来找到本次数据应该读取的位置。从而实现
 * 同步。
 * 第二中方法参考了：https://github.com/Wangzhike/HIT-Linux-0.11
 * 下面的是第二种方法, 第一种有空补上：
 */
int main(int argc, char * argv[]){
    //创建信号量
    sem_t* full = sem_open("/full", O_CREAT, 0666, 0);
    sem_t* mutex = sem_open("/mutex", O_CREAT, 0666, 1);
    sem_t* empty = sem_open("/empty", O_CREAT, 0666, MAX_BUFFER);

    pid_t pid[NR_PROC];
    int tmp=0, position=0;

    int fp = open("file", O_CREAT | O_RDWR | O_TRUNC, 0666);

    for(int i = 0; i < NR_PROC; i++) {// 消费者
        pid_t cur_pid = fork();
        if(cur_pid == 0) { //子进程调用fork后，返回0
            while(1){
                sem_wait(full);
                sem_wait(mutex);

                //读取上一次读取出来的数字
                lseek(fp, 0, SEEK_SET);
                read(fp, &tmp, sizeof(int));
                if(tmp == MAX_NUMBER){
                    sem_post(mutex);
                    sem_post(empty);
                    break;
                }

                //读取本次的数据
                position = ((tmp+1) % MAX_BUFFER) + 1;
                lseek(fp, position*sizeof(int), SEEK_SET);
                read(fp, &tmp, sizeof(int));
                printf("%d: %d\n", getpid(), tmp);
                fflush(stdout);

                //写入本次读取的数据
                lseek(fp, 0, SEEK_SET);
                write(fp, (void*)&tmp, sizeof(int));

                sem_post(mutex);
                sem_post(empty);
            }
//            printf("I'm exit, pid: %d\n", getpid());
            return 0;
        }
    }
    tmp = -1; position = 0;
    lseek(fp, 0, SEEK_SET);
    write(fp, (void*)&tmp, sizeof(int)); //写入标志位
    //这里在循环中加上NR_PROC是为了让子进程能够退出循环。
    for(int i=0; i < MAX_NUMBER+NR_PROC+1; i++){ //生产者
        sem_wait(empty);
        sem_wait(mutex);

        position = (i % MAX_BUFFER) + 1;
        lseek(fp, position*sizeof(int), SEEK_SET);
        write(fp, (void*)&i, sizeof(int));

        sem_post(mutex);
        sem_post(full);
    }

    for(int i = 0; i < NR_PROC; i++){
        wait(&pid[i]);
    }
    close(fp);
    sem_close(full);
    sem_close(mutex);
    sem_close(empty);
//    printf("the father exit\n");
    return 0;
}




