#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <pthread.h>
#include <semaphore.h>

bool safety_algorithm();
void * customer_thread(int n);
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int request[]);

#define NUMBER_OF_RESOURCES 8
#define NUMBER_OF_CUSTOMERS 10
#define MAX_RESOURCES 20

int available[NUMBER_OF_RESOURCES];
int max[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];


sem_t mutex;

int main(int argc, char ** argv) {

    if (argc < NUMBER_OF_RESOURCES)
    {
        printf("not enough arguments\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] = strtol(argv[i + 1], NULL, 10);
    }
    

    sem_init(&mutex, 0, 1);


    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            max[i][j] = rand() % MAX_RESOURCES;
            need[i][j] = max[i][j]; //At first need is equal to max
            allocation[i][j] = 0;
        }
    }
    
    for (int m = 0; m < NUMBER_OF_CUSTOMERS; ++m) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
            need[m][j] = max[m][j] - allocation[m][j];
        }
    }

    

    printf("Available Array:\n");
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
	{
	     printf("av[%d]: %d\n", i, available[i]);
	}
	printf("\n");

	printf("Maximum:\n");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
	    for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
	    {
			     printf("max[%d]: %d\t", i, max[i][j]);
	    }
	    printf("\n");
	}
	printf("Allocation:\n");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
	    for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
	    {
			     printf("alloc[%d]: %d\t", i, allocation[i][j]);
	    }
	    printf("\n");
	}
	printf("Need:\n");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
	{
	    for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
	    {
			     printf("need[%d]: %d\t", i, need[i][j]);
	    }
	    printf("\n");
	}
	printf("******************\n");

pthread_t customer_threads[NUMBER_OF_CUSTOMERS];

    for (int k = 0; k < NUMBER_OF_CUSTOMERS; ++k) {
        pthread_create(&customer_threads[k], NULL, (void *) customer_thread, (void *) k);
    }
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        pthread_join(customer_threads[i], NULL);
    }
    return 0;
}

int request_resources(int customer_num, int request[]){
    bool flag = true;
    for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
        if (request[j] > need[customer_num][j]) {
            perror("Process has exceeded its max");
            return -1;
        }

    }

    sem_wait(&mutex);

    for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
        available[j] -= request[j];
        allocation[customer_num][j] +=request[j];
        need[customer_num][j] -= request[j];
    }

    if (safety_algorithm()){
        sem_post(&mutex);
        return 0;
    } else {
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
            available[j] += request[j];
            allocation[customer_num][j] -=request[j];
            need[customer_num][j] += request[j];
        }
    }
    sem_post(&mutex);
    return -1;
}

int release_resources(int customer_num, int request[]){
    sem_wait(&mutex);
    for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
        available[j] += request[j];
        allocation[customer_num][j] -=request[j];
        need[customer_num][j] += request[j];
    }
    sem_post(&mutex);
return 1;
}

bool safety_algorithm(){
    int work[NUMBER_OF_RESOURCES];
    bool finish[NUMBER_OF_CUSTOMERS];

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        work[i] = available[i];
    }
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        finish[i] = false;
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        if (!finish[i]){
            bool flag = true;
            for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
                if (need[i][j] > work[j]){
                    flag = false;
                }
            }
		if(flag) {
			for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {

		            work[i] = work[i] + allocation[i][j];
		            
		        }
			finish[i] = true;
		}
           
        }
    }
    for (int k = 0; k < NUMBER_OF_CUSTOMERS; ++k) {
        if (!finish[k]) {
            return false;
        }
    }

    return true;

}

void * customer_thread(int n){
    srand(time(NULL));
    int request[NUMBER_OF_RESOURCES];
for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
    int type = rand() % 2;
    if (type == 0) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
            request[j] = rand() % (need[n][j]+1);
        }
        
	printf("Costumer %d has requested %s\b \n\t" , n, request);
	printf("\tResult: %s", request_resources(n, request) ==0 ? "Accepted -> Resource Allocated": "Not Accepted : Resource Not Allocated\n");
	  
    } else {
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
            request[j] = rand() % (allocation[n][j]+1);
        }
        release_resources(n, request);
printf("Costumer %d has released %s\n" , n, request);
    }
}

}

	
	
	

