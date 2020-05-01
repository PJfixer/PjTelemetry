
#include "serial.h"

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

int open_port(char * port)
{
  int fd; /* File descriptor */


  fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {

    char str[50];
    sprintf(str, "open_port: Unable to open %s \n ", port);
    perror(str);
  }
  else
  {
    fcntl(fd, F_SETFL, 0);
  }
  return (fd);
}

