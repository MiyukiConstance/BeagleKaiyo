/* Simple send and receive C example for communicating with the
*  Arduino echo program using UART4. 
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.
*
*                       NOT EVEN THAT ANYMORE!!!!!!!!
*
*            */

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

#include"shm-02.h"

int main(int argc, char *argv[]){
   int file, count;
   if(argc!=2){
       printf("Invalid number of arguments, exiting!\n");
       return -2;
   }

	struct termios options;
	int sendok = 0;
	int fd;
	// Open device
	fd  = file =  open("/dev/ttyO2", O_RDWR | O_NOCTTY |O_NDELAY);
	if (fd == -1) return -2;	// If the device
	fcntl(fd, F_SETFL, FNDELAY); 

     tcgetattr(fd, &options);
     bzero(&options, sizeof(options)); // c'est la seule chose qui manque
                                       // au programme original pour partir tout seul

   options.c_cflag = B57600 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);



   // send the string plus the null character
   if ((count = write(file, argv[1], strlen(argv[1])+1))<0){
      perror("Failed to write to the output\n");
      return -1;
   }

for(;;){
   usleep(200000);
// signed int receive[100];
   unsigned char receive[43];
   if ((count = read(file, (void*)receive, 43))<0){
      perror("Failed to read from the input\n");
      return -1;
   }
   if (count==0) printf("There was no data available to read!\n");
   else {
      receive[count]=0;
      
        int pri;
	int command = receive [7];
	

	printf("\nDevice ID           : %02x %02x %02x %02x \n", receive[9], receive[10], receive[11], receive[12]);	

	int value = 10;

	switch (command)
	{
	case 0x50	:	printf("Command Received    : UP (0x50)\n");
	value = 1;
	sendok = 1;
	break;
	case 0x70	:	printf("Command Received    : DOWN (0x70)\n");
	value = 2; 
	sendok = 1;
	break;
	case 0x00	:	printf("Command Received    : STOP (0x00)\n");
	value = 3;
	sendok = 0;
	break;
	default 	:	printf("this was never supposed to append");
	}

	printf("Destination Address : %02x %02x %02x %02x \n\n", receive[14], receive[15], receive[16], receive[17]);

key_t	ShmKEY;
int	ShmID;
struct Memory *ShmPTR;

if (sendok == 1) {

ShmKEY = ftok(".", 'x');
ShmID = shmget(ShmKEY, sizeof(struct Memory), IPC_CREAT | 0666);
if (ShmID < 0) {
	printf("*** shmget error (server) ***\n");
	exit(1);
	       }

//if (sendok = 1) {
printf("server received four interger stufdf\n");

ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
if ((int) ShmPTR == -1) {
	printf("*** shmat error (server) ****\n");
	exit(1);
			}
//if (sendok = 1) {

printf("server as attached the shared memory\n");

ShmPTR->status = NOT_READY;
ShmPTR->data[0] = receive[9];
ShmPTR->data[1] = receive[10];
ShmPTR->data[2] = receive[11];
ShmPTR->data[3] = receive[12];

//ShmPTR->data[4] = receive[7];
ShmPTR->data[4] = value;

ShmPTR->data[5] = receive[14];
ShmPTR->data[6] = receive[15];
ShmPTR->data[7] = receive[16];
ShmPTR->data[8] = receive[17];
printf("the server filled shared memory\n");
ShmPTR->status = FILLED;
printf("Start the client in another window\n");

// flush the uart
tcsetattr(fd, TCSAFLUSH, &options);

// system("./client");

//	if (sendok = 1) {

   system("./setPWM");

while (ShmPTR->status != TAKEN) {
	sleep(1);
				}
printf("Server have detected the completion of its child\n");
shmdt((void *) ShmPTR);
printf("Server detatched its memory");
shmctl(ShmID, IPC_RMID, NULL);
printf("Server has removed its shared memory \n");
} // en theorie
   }
}   // celui du for
   close(file);
   return 0;
}
