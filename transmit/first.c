#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
int shmid;
int shmkey = 12222;

int main ()
{
printf("poulet \n");
shmid = shmget(shmkey,1024,IPC_CREAT);
char *shmpointer = shmat(shmid, NULL);
shmdt(&shmpointer, NULL);
}
