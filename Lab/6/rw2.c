#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <sys/shm.h>
#include <sys/stat.h>

sem_t mutex,writeblock;
int rcount = 0;
char buffer[1];

int segment_id ;
char *shared_memory;

void *reader(void *arg)
{
  sem_wait(&mutex);
  rcount = rcount + 1;
  if(rcount==1)
   sem_wait(&writeblock);
  sem_post(&mutex);
  printf("Data read by the reader is %d\n",(buffer[0]-'0'));
  sleep(1);
  sem_wait(&mutex);
  rcount = rcount - 1;
  if(rcount==0)
   sem_post(&writeblock);
  sem_post(&mutex);

}

void *writer(void *arg)
{
int curr = (buffer[0] -'0');

if(curr<10){
  sem_wait(&writeblock);
	
  curr++;
  buffer[0] = curr +'0';
  sprintf(shared_memory, (char *) buffer);
  printf("Data writen by the writer is %d\n",curr);
  sleep(1);
  sem_post(&writeblock);
}
else 
  sem_wait(&mutex);
}

int main()
{
  int i,b; 
  segment_id = shmget(IPC_PRIVATE, 1000, S_IRUSR | S_IWUSR);
  if(segment_id == -1)
	perror("shared mem error");
  shared_memory = (char *) shmat(segment_id, NULL, 0);

buffer[0] =  0 +'0';
  pthread_t rtid[2],wtid;
  sem_init(&mutex,0,1);
  sem_init(&writeblock,0,1);
while(1<2){
  pthread_create(&wtid,NULL,writer,(void *)0);
  pthread_create(&rtid[0],NULL,reader,(void *)0);
  pthread_create(&rtid[1],NULL,reader,(void *)1);

  pthread_join(wtid,NULL);
  pthread_join(rtid[0],NULL);
  pthread_join(rtid[1],NULL);
}
shmdt(shared_memory);
shmctl(segment_id, IPC_RMID, 0);
  return 0;
}




