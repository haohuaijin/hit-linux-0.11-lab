#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
/*
 * shmget() 会新建/打开一页内存，并返回该页共享内存的 shmid（该块共享内存在操作系统内部的 id）。
 * 所有使用同一块共享内存的进程都要使用相同的 key 参数。
 * 如果 key 所对应的共享内存已经建立，则直接返回 shmid。
 * 如果 size 超过一页内存的大小，返回 -1，并置 errno 为 EINVAL。
 * 如果系统无空闲内存，返回 -1，并置 errno 为 ENOMEM。
 * shmflg 参数可忽略。
 */
int shmget(key_t key, size_t size, int shmflg);
/*
 * shmat() 会将 shmid 指定的共享页面映射到当前进程的虚拟地址空间中，并将其首地址返回。
 * 如果 shmid 非法，返回 -1，并置 errno 为 EINVAL。
 * shmaddr 和 shmflg 参数可忽略。
 */
void *shmat(int shmid, const void *shmaddr, int shmflg);




















