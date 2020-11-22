#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <linux/sched.h>
#include <a.out.h>
#include <linux/kernel.h>

int sys_shmget(int key, size_t size, int shmflg);
int sys_shmat(int shmid, const void *shmaddr, int shmflg);

/*
 * shmget() 会新建/打开一页内存，并返回该页共享内存的 shmid（该块共享内存在操作系统内部的 id）。
 * 所有使用同一块共享内存的进程都要使用相同的 key 参数。
 * 如果 key 所对应的共享内存已经建立，则直接返回 shmid。
 * 如果 size 超过一页内存的大小，返回 -1，并置 errno 为 EINVAL。
 * 如果系统无空闲内存，返回 -1，并置 errno 为 ENOMEM。
 * shmflg 参数可忽略。
 */
int allKey[20];
void *allAddress[20];
int countKey;
int sys_shmget(int key, size_t size, int shmflg){
    int i;

    for(i=0; i<countKey; i++)
        if(allKey[i] == key)
            return key; 

    if(size > 4096){
        errno = EINVAL;
        return -1;
    }

    int tmp = get_free_page();
    if(tmp == 0){
        errno = ENOMEM;
        return -1;
    }

    allKey[countKey] = key;
    allAddress[countKey] = (void*)tmp;
    countKey++;

    return key; 
}
/*
 * shmat() 会将 shmid 指定的共享页面映射到当前进程的虚拟地址空间中，并将其首地址返回。
 * 如果 shmid 非法，返回 -1，并置 errno 为 EINVAL。
 * shmaddr 和 shmflg 参数可忽略。
 */
int sys_shmat(int shmid, const void *shmaddr, int shmflg){
    unsigned long data_limit, data_base;
	int i, j;
    void * result;

    for(i=0; i<countKey; i++)
        if(allKey[i] == shmid)
            break;
    if(i == countKey) return -1;

    result = allAddress[i];
	data_limit = 0x4000000;
	data_base = get_base(current->ldt[1]);

    data_base += data_limit;
    data_base -= PAGE_SIZE;
    put_page(result, data_base);
    
    return result;
}