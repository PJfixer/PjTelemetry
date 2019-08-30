/*
 * main.c
 * 
 * Copyright 2019 PiAir <piair@debian>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include "serial.h"
#include "udp.h"
#include <pthread.h>
#include <sys/types.h> /*  linux sys types */
#include <sys/time.h> /* linux sys time struct*/
#include <sys/select.h> /* select fd */
#include "bridge.h"
extern int sockfd; 
extern struct sockaddr_in servaddr;
int port;
extern bool SerialProcess;
extern bool UdpProcess;

int main(int argc, char **argv)
{
	fd_set input;
	int max_fd;
	struct timeval timeout;
	port = open_port();
	if(port != -1)
	{
		if(!open_connection("127.0.0.1",8000))
		{
			while(1)
			{
				int n;
				FD_ZERO(&input);
				FD_SET(port, &input);
				FD_SET(sockfd, &input);
				max_fd = (sockfd > port ? sockfd : port) + 1;
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
						if(SerialProcess == false)
						{
							// then run the serial Thread
						}
					}
				  if (FD_ISSET(sockfd, &input))
					{
					 fprintf(stdout,"DATA sur UDP \n"); 
						if(UdpProcess == false)
						{
							// then run the udpthread
						}
					}
				 
				}
				else
				{
				  fprintf(stdout,"TIMEOUT \n");
				  
				}
			}
		}
	}
	
	
	
	
	return 0;
}

