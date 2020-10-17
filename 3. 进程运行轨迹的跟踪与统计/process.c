#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

#include <stdlib.h>

#define HZ	100

void cpuio_bound(int last, int cpu_time, int io_time);

#define NR_PROC 5
int main(int argc, char * argv[])
{
	pid_t pid[NR_PROC];

    for(int i = 0; i < NR_PROC; i++) {
        pid_t cur_pid = fork();
        if(cur_pid == 0) { //子进程调用fork后，返回0
			cpuio_bound(2*(i+1), 0, 1);
			printf("the pid is %d, the father pid is %d.\n", getpid(), getppid());
			return 0; //退出子进程
        } else { //父进程返回的是子进程的pid
			pid[i] = cur_pid;
			printf("the %d child pid is %d\n", i, pid[i]);
		}
    }
	/*
    for(int i = 0; i < NR_PROC; i++){
		wait(&pid[i]);
	}
	*/
	printf("the parent is finished.\n");
	return 0;
}

/*
 * 此函数按照参数占用CPU和I/O时间
 * last: 函数实际占用CPU和I/O的总时间，不含在就绪队列中的时间，>=0是必须的
 * cpu_time: 一次连续占用CPU的时间，>=0是必须的
 * io_time: 一次I/O消耗的时间，>=0是必须的
 * 如果last > cpu_time + io_time，则往复多次占用CPU和I/O
 * 所有时间的单位为秒
 */
void cpuio_bound(int last, int cpu_time, int io_time)
{
	struct tms start_time, current_time;
	clock_t utime, stime;
	int sleep_time;

	while (last > 0)
	{
		/* CPU Burst */
		times(&start_time);
		/* 其实只有t.tms_utime才是真正的CPU时间。但我们是在模拟一个
		 * 只在用户状态运行的CPU大户，就像“for(;;);”。所以把t.tms_stime
		 * 加上很合理。*/
		do
		{
			times(&current_time);
			utime = current_time.tms_utime - start_time.tms_utime;
			stime = current_time.tms_stime - start_time.tms_stime;
		} while ( ( (utime + stime) / HZ )  < cpu_time );
		last -= cpu_time;

		if (last <= 0 )
			break;

		/* IO Burst */
		/* 用sleep(1)模拟1秒钟的I/O操作 */
		sleep_time=0;
		while (sleep_time < io_time)
		{
			sleep(1);
			sleep_time++;
		}
		last -= sleep_time;
	}
}

