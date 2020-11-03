#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

volatile int counter = 0; 
int loops;
sem_t *full;

void *worker(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        sem_wait(full);
	    counter++;
        sem_post(full);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    full = sem_open("/full_sem", O_CREAT, 0666, 1);
    if (full == SEM_FAILED) {
        sem_close(full);
        printf("Failed to open semphore for full");
    }
    loops = 1000000;
    pthread_t p1, p2;
    printf("Initial value : %d\n", counter);
    pthread_create(&p1, NULL, worker, NULL); 
    pthread_create(&p2, NULL, worker, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("Final value   : %d\n", counter);
    return 0;
}

