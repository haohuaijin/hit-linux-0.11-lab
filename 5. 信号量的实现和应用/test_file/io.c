#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char * argv[]){
    char buf[10] = {0};
    int fpt = open("bfr", O_CREAT|O_RDWR|O_TRUNC|O_APPEND, 0666);
    for(int i=0; i<10; i++){
        sprintf(buf, "%-5d\n", i);//将数字输入到字符串
        write(fpt, buf, 6);
    } 
    close(fpt);

    fpt = open("bfr", O_RDONLY);
    for(int i=0; i<10; i++){
        int byte = read(fpt, buf, 6);
        printf("the byte of read is %d\n", byte);
        printf("%s", buf);
    }
    close(fpt);
	return 0;
}
