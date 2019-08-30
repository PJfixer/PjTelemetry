   // #include <stdio.h>   /* Standard input/output definitions */
	//#include <string.h>  /* String function definitions */
	//#include <unistd.h>  /* UNIX standard function definitions */
	//#include <fcntl.h>   /* File control definitions */
	//#include <errno.h>   /* Error number definitions */
	//#include <termios.h> /* POSIX terminal control definitions */
	//#include <stdlib.h>
	//#include <unistd.h>
	#include <sys/types.h> /*  linux sys types */
	#include <sys/time.h> /* linux sys time struct*/
	#include <sys/select.h> /* select fd */
	#include "serial.h"
	
    int main()
    {
       printf("Serial with select pjeanne 29/08/2019");
       int port = open_port();
       

       if(port != -1) // si succes
       {
		   set_baudrate(port,115200);
			int            n;
			int            socket;
			int            max_fd;
			fd_set         input;
			struct timeval timeout;
			/* Initialize the input set */
			
			
			while(1)
			{
			   /* Initialize the timeout structure */
				FD_ZERO(&input);
				FD_SET(port, &input);
				max_fd = (port+1);
				timeout.tv_sec  = 10;
				timeout.tv_usec = 0;

				/* Do the select */
				n = select(max_fd,&input,  NULL, NULL,&timeout) ;

				/* See if there was an error */
				if (n == -1)
				{
				  perror("select failed");
				}
				else if (n)
				{
				   /* We have input */
				  if (FD_ISSET(port, &input))
					{
					 fprintf(stdout,"DATA sur SERIAL \n"); 
					 char * buf;
					 int d = read(port,&buf,30);
					 fprintf(stdout,"read %d bytes from serial\n",d); 
					 write(1,&buf,d);
					 
					 
					}
				 
				}
				else
				{
				  fprintf(stdout,"TIMEOUT \n");
				   write(port,"*IDN?\r\n",7); 
				}

			}
	   }
	   else
	   {
		   fprintf(stderr,"can't open serial port exiting now! \n");
		   exit(-1);
	   }
       return 0;
    }
    





