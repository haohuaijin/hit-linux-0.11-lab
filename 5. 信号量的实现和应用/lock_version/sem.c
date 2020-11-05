#include <errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <asm/system.h>
#include <unistd.h>
#include <asm/segment.h>

int sys_sem_open(const char *name, unsigned int value);
int sys_sem_wait(sem_t *sem);
int sys_sem_post(sem_t *sem);
int sys_sem_unlink(const char *name);

sem_tx total_sem[20];
int count_sem;

int sys_sem_open(const char *name, unsigned int value){
    int i, count;
    char str[20];

    count = 0;
    do{ 
        str[count] = get_fs_byte(name + count);
    } while (count <= 20 && str[count++] != '\0');

    for(i=0; i<count_sem; i++){
        if(!strcmp(total_sem[i].name, str)){
            return i;
        }
    } 

    total_sem[count_sem].value = value;
    /*strcmp(total_sem[count_sem].name, str);*/

    for(i=0; i<count; i++)
        total_sem[count_sem].name[i] = str[i];
    total_sem[count_sem].name[i] = '\0'; 
    //printk("the open name is %s, name is %s.\n", total_sem[count_sem].name, str);

    total_sem[count_sem].quence = NULL;
    total_sem[count_sem].lock = 0;
    count_sem++;

    return (count_sem-1);
}


int sys_sem_wait(sem_t *sem){
    cli();
    int i;
    /*printk("the wait is %d, pid is %d.\n", (int)sem, current->pid);*/
    for(i=0; i<count_sem; i++){
        if(i == (int)sem){
            while(total_sem[i].lock)
                sleep_on(&total_sem[i].quence);
            total_sem[i].lock = 1;
        }
    }
    sti();
    return 0;
}

int sys_sem_post(sem_t *sem){
    cli();
    int i;
    /*printk("the post is %d, pid is %d.\n", (int)sem, current->pid);*/
    for(i=0; i<count_sem; i++){
        if(i == (int)sem){
            /* while error in first post empty in P
            if(!total_sem[i].lock)
                printk("sem error!\n");
            */
            total_sem[i].lock = 0;
            wake_up(&total_sem[i].quence);
        }
    }
    sti();
    return 0;
}

int sys_sem_unlink(const char *name){
    int i,j;
    char str[20];

    i = 0;
    do{ 
        str[i] = get_fs_byte(name + i);
    } while (i <= 20 && str[i++] != '\0');

    for(i=0; i<count_sem; i++){
        /* 
        printk("the name is %s, name is %s, the i is %d.\n", 
                total_sem[i].name, str, i);
        */
        if(!strcmp(total_sem[i].name, str)){
            break; 
        }
    }

    if(i == count_sem){
        printk("No sem of the %s name.\n", str);
        return -1;
    }
    for(j=i; j<count_sem; j++){
        total_sem[j] = total_sem[j+1];
    } 
    count_sem--;
    return 0;
}