#include <asm/segment.h>
#include <errno.h>
#include <string.h>

char myname[24];

int sys_iam(const char *name){
    char str[25];
    int i = 0;

    do{ // get char from user input
        str[i] = get_fs_byte(name + i);
    } while (i <= 25 && str[i++] != '\0');

    if (i > 24){
        return -(EINVAL);
    }
    strcpy(myname, str);

    return i-1;
}

int sys_whoami(char *name, unsigned int size){
    int length = strlen(myname);

    if (size < length){
        return -(EINVAL);
    }
    int i = 0;
    for (i = 0; i < length; i++)
    {
        // copy from kernel mode to user mode
        put_fs_byte(myname[i], name + i);
    }
    return length;
}