#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <semaphore.h>

#define SHM_KEY 102030
#define SHM_SIZE sizeof(int) * 25

sem_t mutex;

int create_segment() {
    int segment_id = shmget (IPC_PRIVATE, 1000,S_IRUSR | S_IWUSR);
    if (segment_id == -1) {
        perror("shmget");
    }
    return segment_id;
}

int* attach_segment(int shmid) {
    int *shm;

    if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
        printf("Shared memory did not attached to address!");
    }

    return shm;
}
void detach_segment(int *shm) {
    shmdt(shm);
}
void remove_segment(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        printf("Couldn't remove the shared memory!");
    }
}

void print_histogram(int *hist, int number_of_samples) {
    printf("Histogram for sample %d:\n", number_of_samples);
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < hist[i]; j++) {
            printf("*");
        }
        printf("\n");
    }
}

double calculate(int number_of_samples) {
    clock_t begin = clock();
    int shmid = create_segment();
    int * hist = attach_segment(shmid);
    srand(time(0));
    int rand_num, counter;
    if(sem_init(&mutex, 1, 1)<0){
        perror("error in semaphore initialization ");
        exit(0);
    }

    pid_t pid = fork();
    if (pid == 0){
        sem_post(&mutex);
        for (int i = 0; i < number_of_samples / 2; i++){
            counter = 0;
            for (int j = 0; j < 12; j++) {
                rand_num = rand() % 100;
                while (rand_num < 0) {
                    rand_num = rand() % 100;
                }
                if (rand_num >= 49)
                    counter +=1;
                else
                    counter -=1;
            }
            hist[counter+12] += 1;
        }
        exit(0);
    }
    else {
        sem_wait(&mutex);
        for (int i = 0; i < number_of_samples / 2; i++) {
            counter = 0;
            for (int j = 0; j < 12; j++) {
                rand_num = rand() % 100;
                while (rand_num < 0) {
                    rand_num = rand() % 100;
                }
                if (rand_num >= 49)
                    counter += 1;
                else
                    counter -= 1;
            }
            hist[counter + 12] += 1;
        }
        print_histogram(hist, number_of_samples);
        sleep(5);
    }
    sem_post(&mutex);
    wait((int *)0);
    sem_destroy(&mutex);
    detach_segment(hist);
    clock_t end = clock();

    double time_spend = (double ) (end-begin)/ CLOCKS_PER_SEC;

    return time_spend;
}

int main() {
    double time_spend = calculate(5000);
    printf("%f", time_spend);
    return 0;
}