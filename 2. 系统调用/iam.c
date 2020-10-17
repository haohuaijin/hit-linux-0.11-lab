#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

//iam()在用户空间的接口函数
// 这是定义在unistd.h里面的一个宏，展开后是一个包含int 0x80中断的代码。
_syscall1(int, iam, const char*, name);

int main(int argc, char* argv[]) {
    iam(argv[1]);
    return 0;
}