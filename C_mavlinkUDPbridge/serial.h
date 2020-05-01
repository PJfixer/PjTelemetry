#ifndef SERIAL_H

#define SERIAL_H
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>
#include <unistd.h>
int open_port(char * port);
void set_baudrate(int fd,unsigned int speed);

#endif
