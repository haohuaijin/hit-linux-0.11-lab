#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd;
    int ar[5] = {30, 40, 500, 67, 23};
    off_t off;
    int retValue;

    fd = open("out", O_CREAT|O_RDWR, 0666);

    if(fd == -1){
        perror("open");
        exit(-1);
    }

    write(fd, (void*) ar, sizeof(ar));

    off = lseek(fd, 2*sizeof(int), SEEK_SET);

    if(off == -1){
        perror("lseek");
        close(fd);
        exit(-1);
    }

    read(fd, &retValue, sizeof(int));

    printf("the return value is %d\n", retValue);
    return 0;
}