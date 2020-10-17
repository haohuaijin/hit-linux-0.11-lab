#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

// whoami()在用户空间的接口函数
// 这是定义在unistd.h里面的一个宏，展开后是一个包含int 0x80中断的代码。
_syscall2(int, whoami,char*,name,unsigned int,size);

int main(char *arg) {
    char name[30];
    whoami(name, 30);
  	printf("%s\n", name);
    return 0;
}