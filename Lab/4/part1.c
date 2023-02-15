#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
int main ()
{
    int segment_id;
    char* shared_memory;


    segment_id = shmget (IPC_PRIVATE, 1000,S_IRUSR | S_IWUSR);
    shared_memory = (char*) shmat(segment_id, NULL, 0);
    pid_t pid = fork();

    if (pid < 0){
        printf("failed to create process");
        exit(EXIT_FAILURE);

    } else if (pid == 0){
        sprintf(shared_memory, "Writing to shared memory from process1\n");
        exit(EXIT_SUCCESS);

    } else {
        waitpid(pid, NULL, WUNTRACED | WCONTINUED);
        printf("process 2: \n");
        printf("Data read from shared mememory: %s\n", shared_memory);
    }
    shmdt(shared_memory);
    shmctl (segment_id, IPC_RMID, 0);
    return 0;
}
