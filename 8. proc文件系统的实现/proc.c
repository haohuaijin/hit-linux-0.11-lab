#include <linux/kernel.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <linux/sched.h>
#include <asm/segment.h>
#include <linux/fs.h>

int sprintf(char *buf, const char *fmt, ...){
    va_list args; int i;
    va_start(args, fmt);
    i=vsprintf(buf, fmt, args);
    va_end(args);
    return i;
}

int proc_read(unsigned short dev, char *buf, int count, off_t *pos){
    int res = 0;
    /* for psinfo */
    if(dev == 0){
        int total_len= 0;
        int len = 0;
        int flag = 0;
        char *infobuf = (char*)malloc(sizeof(char)*600);
        char *tbuf = (char*)malloc(sizeof(char)*120);

        memset(infobuf, 0, strlen(infobuf));
        memset(tbuf, 0, strlen(tbuf));
        struct task_struct **p;
        len = sprintf(tbuf, "pid    state    father    counter    start_time\n");
        total_len += len;
        if(total_len >= *pos){ //找到上一次读取到的地方
            flag = 1;
            strncpy(infobuf, tbuf, total_len-(*pos));
            total_len = total_len - (*pos);
        }

        for(p = &FIRST_TASK; p <= &LAST_TASK; ++p){
            if(*p){
                len = sprintf(tbuf, "%ld\t%ld\t%ld\t%ld\t%ld\n", (*p)->pid, (*p)->state, (*p)->father, (*p)->counter, (*p)->start_time);
                total_len += len;
                if(flag) 
                    strncpy(infobuf+strlen(infobuf), tbuf, strlen(tbuf));
                if(total_len >= *pos && !flag){ //找到上一次读取到的地方
                    flag = 1;
                    strncpy(infobuf, tbuf+len-(total_len-(*pos)), total_len-(*pos));
                    total_len = total_len - (*pos);
                }
                if(total_len > count && flag)
                    break;
            }
        }

        int i, temp;
        temp = total_len > count ? count : total_len;
        for(i=0; i < temp; i++){
            put_fs_byte(infobuf[i], buf + i);
        }
        res = temp;
        *pos += temp;
        free(infobuf);
        free(tbuf);
    }
    /* for hdinfo */
    if(dev == 1){
        int len = 0;
        int total_len = 0;
        int total_blocks = 0;
        int free_blocks = 0;
        int i, j, k;
        struct super_block * sb;
        struct buffer_head * bh;
        char *hdbuf = (char*)malloc(sizeof(char)*500); 

        sb = get_super(current->root->i_dev);
        total_blocks = sb->s_nzones;
        total_len += sprintf(hdbuf, "total_blocks:\t%d\n", total_blocks);

        /* 通过位图获得空闲数据块的信息 */
        for(i=0; i<sb->s_zmap_blocks; i++){
            bh = sb->s_zmap[i];
            char *p = (char *)bh->b_data;
            for(j = 0; j<1024; j++){
                unsigned char temp = p[j]; 
                temp = ~temp;
                for(k=0; k<8; k++){
                    free_blocks += temp & 0x1;
                    temp >>= 1;
                }
            }
        }

        total_len += sprintf(hdbuf+strlen(hdbuf), "free_blocks:\t%d\n", free_blocks);
        total_len += sprintf(hdbuf+strlen(hdbuf), "user_blocks:\t%d\n", total_blocks - free_blocks);
        total_len += sprintf(hdbuf+strlen(hdbuf), "imap_blocks:\t%d\n", sb->s_imap_blocks);
        total_len += sprintf(hdbuf+strlen(hdbuf), "zmap_blocks:\t%d\n", sb->s_zmap_blocks);
 
        for(i=0; i<total_len; i++){
            put_fs_byte(hdbuf[i], buf+i);
        }

        if(total_len == *pos){
            res = 0;
        } else {
            *pos += total_len;
            res = total_len;
        }
        free(hdbuf);
    }
    return res;
}
