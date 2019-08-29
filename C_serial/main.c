    #include <stdio.h>   /* Standard input/output definitions */
	#include <string.h>  /* String function definitions */
	#include <unistd.h>  /* UNIX standard function definitions */
	#include <fcntl.h>   /* File control definitions */
	#include <errno.h>   /* Error number definitions */
	#include <termios.h> /* POSIX terminal control definitions */
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/types.h> /*  linux sys types */
	#include <sys/time.h>v /* linux sys time struct*/
	#include <sys/select.h> /* select fd */

	int open_port(void);
	void set_baudrate(int fd,unsigned int speed);
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
    
void set_baudrate(int fd,unsigned int speed)
{
	
	struct termios config; // declaration d'un structure termios
	tcgetattr(fd, &config); // get current port state and store in termios struct
	switch(speed)
	{
		case 9600:
			cfsetispeed(&config, B9600); // set baudrate RX to 9600
			cfsetospeed(&config, B9600); // set baudrate TX to 9600
		break;
		case 19200:
			cfsetispeed(&config, B19200); // set baudrate RX to 19200
			cfsetospeed(&config, B19200); // set baudrate TX to 19200
		break;
		case 57600:
			cfsetispeed(&config, B57600); // set baudrate RX to 57600
			cfsetospeed(&config, B57600); // set baudrate TX to 57600
		break;
		case 115200:
			cfsetispeed(&config, B115200); // set baudrate RX to 115200
			cfsetospeed(&config, B115200); // set baudrate TX to 115200
		break;
		default:
			cfsetispeed(&config, B9600); // set baudrate RX to 9600
			cfsetospeed(&config, B9600); // set baudrate TX to 9600
		break;
	}
	
	config.c_cflag |= (CLOCAL | CREAD); // enable receiver & put in local mode
	config.c_cflag &= ~CSIZE; /* Mask the character size bits */
	config.c_cflag |= CS8;    /* Select 8 data bits */
	//set 8N1 no parity
	config.c_cflag &= ~PARENB;
	config.c_cflag &= ~CSTOPB;
	config.c_cflag &= ~CSIZE;
	config.c_cflag |= CS8;
	
	//disable flow control
	//config.c_cflag &= ~CNEW_RTSCTS; //if supported
	config.c_lflag |= (ICANON ); //input is line-oriented. Input characters are put into a buffer  
	//config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // raw
	//until a CR (carriage return) or LF (line feed) character is received. 
	config.c_iflag &= ~(IXON | IXOFF | IXANY ); // disable software flow control
	config.c_iflag |= IGNCR ; 
	config.c_oflag |= (OPOST);

	tcsetattr(fd, TCSANOW, &config); // write new parameter NOW! to serial port
	
}

int open_port(void)
{
  int fd; /* File descriptor */


  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {


    perror("open_port: Unable to open /dev/ttyUSB0 - ");
  }
  else
    fcntl(fd, F_SETFL, 0);

  return (fd);
}





