#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <sys/mman.h>
#include <semaphore.h>

int read_count = 0;


sem_t mutex,writeblock;
int rcount = 0;


int segment_id ;
char *shared_memory;
char buffer[1];



void reader_writer(pid_t pid1, pid_t pid2){

    if (pid1 < 0 || pid2 < 0){
        printf("failed to create process");
        exit(EXIT_FAILURE);

    } else if (pid1 == 0 || pid2 == 0){
            sem_wait(&mutex);
            rcount = rcount + 1;
            if(rcount==1)
                sem_wait(&writeblock);
            sem_post(&mutex);
            printf("Data read by the reader %d is %s\n",getpid(),(shared_memory));
            sleep(1.2);
            sem_wait(&mutex);
            rcount = rcount - 1;
            if(rcount==0)
                sem_post(&writeblock);
            sem_post(&mutex);

    } else if (pid1 > 0 || pid2 > 0) {
            int curr = (buffer[0] -'0');

            if(curr<=9){
                sem_wait(&writeblock);

                curr++;
                buffer[0] = curr +'0';
                sprintf(shared_memory, (char *) buffer);
                printf("Data writen by the writer%d is %d\n",getpid(),curr);
                sleep(1);
                sem_post(&writeblock);
            }
            else {
                sem_wait(&mutex);
    }
}

}
int main() {
    int segment_id = shmget(IPC_PRIVATE, 1000, S_IRUSR | S_IWUSR);
    if(segment_id == -1)
        perror("shared mem error");
    shared_memory = (char *) shmat(segment_id, NULL, 0);
    buffer[0] =  0 +'0';
    sem_init(&mutex,0,1);
    sem_init(&writeblock,0,1);
pid_t pid1;
pid_t pid2;
    pid1 = fork();
    pid2 = fork();

    while(2<3){
        reader_writer(pid1, pid2);
    }
    shmdt(shared_memory);
    shmctl(segment_id, IPC_RMID, 0);
    return 0;
}




	
	
	

