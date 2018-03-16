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
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

int main(int argc, char *argv[]){
   int file, count;
   if(argc!=2){
       printf("Invalid number of arguments, exiting!\n");
       return -2;
   }
//   if ((file = open("/dev/ttyO2", O_RDWR | O_NOCTTY | O_NDELAY))<0){
//  perror("UART: Failed to open the file.\n");
//    return -1;
//   }

	struct termios options;

	int fd;
	// Open device
	fd  = file =  open("/dev/ttyO2", O_RDWR | O_NOCTTY |O_NDELAY);
	if (fd == -1) return -2;	// If the device
	fcntl(fd, F_SETFL, FNDELAY); 

//   struct termios options;
//   tcgetattr(file, &options);
     tcgetattr(fd, &options);
     bzero(&options, sizeof(options)); // c'est la seule chose qui manque
// au programme original pour partir tout seul

   options.c_cflag = B57600 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
//     options.c_iflag = IGNPAR | IGNBRK;  
   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);

//   cfsetispeed =(B57600);


   // send the string plus the null character
   if ((count = write(file, argv[1], strlen(argv[1])+1))<0){
      perror("Failed to write to the output\n");
      return -1;
   }
   usleep(2000000);
// signed int receive[100];
   unsigned char receive[43];
   if ((count = read(file, (void*)receive, 43))<0){
      perror("Failed to read from the input\n");
      return -1;
   }
   if (count==0) printf("There was no data available to read!\n");
   else {
      receive[count]=0;  //There is no null character sent by the Arduino
//      printf("The following was read in [%d]: %s\n",count,receive);
//      printf("The following was read in [%d]: %s\n",count,receive);
     int pri;
//     for(pri = 0; pri < 43; pri ++){
//	 printf("%d %2x\n", pri, receive[pri]);
//}
	int command = receive [7];
	

	printf("\nDevice ID           : %02x %02x %02x %02x \n", receive[9], receive[10], receive[11], receive[12]);	

	switch (command)
	{
	case 0x50	:	printf("Command Received    : UP (0x50)\n"); break;
	case 0x70	:	printf("Command Received    : DOWN (0x70)\n"); break;
	case 0x00	:	printf("Command Received    : STOP (0x00)\n"); break;
	default 	:	printf("fuck");
	}

	printf("Destination Address : %02x %02x %02x %02x \n\n", receive[14], receive[15], receive[16], receive[17]);
   }
   close(file);
   return 0;
}
