/*
 * main.c
 * 
 * Copyright 2019 Pierre JEANNE <pierre.jeanne96@gmail.com>
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
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/types.h> /*  linux sys types */
#include <sys/time.h> /* linux sys time struct*/
#include <sys/select.h> /* select fd */
#include <string.h>

#include "serial.h"
#include "udp.h"
#include "bridge.h"
extern int sockfd; 
extern struct sockaddr_in servaddr;
int port;
extern volatile bool SerialProcess;
extern volatile bool UdpProcess;

pthread_t ThreadSer ;
pthread_t ThreadUDP ;

extern pthread_mutex_t conditionSer_locker ;
extern pthread_cond_t conditionSer ;

extern pthread_mutex_t conditionUdp_locker ;
extern pthread_cond_t conditionUdp ;

extern pthread_mutex_t conditionSer_done_locker;
extern pthread_cond_t conditionSer_done; 

extern pthread_mutex_t conditionUdp_done_locker;
extern pthread_cond_t conditionUdp_done;


int main(int argc, char *argv[])
{
	char *uart_dev = "/dev/ttyUSB0" ;
	int uart_baudrate =115200 ;
	char *destination_address = "127.0.0.1" ;
	int destination_port = 8000;

	extern char *optarg;
	extern int optind;
	int c;
	
	printf("my process ID is : %d \n\n \n",getpid());
	fd_set input;
	int max_fd;
	struct timeval timeout;

	while ( (c=getopt(argc,argv,"u:b:d:p:")) != EOF )
		switch (c) {
		case 'u':
			uart_dev = optarg;
			break;
		case 'b':
			uart_baudrate = atoi(optarg);
			break;
		case 'd':
			destination_address = optarg;
			break;
		case 'p':
			destination_port = atoi(optarg);
			break;
		
		case '?':
			
			exit(1);
		}

	printf("uart = %s \n",uart_dev);
	printf("uart baudrate = %d \n",uart_baudrate);
	printf("destination address = %s \n",destination_address);
	printf("destination port = %d \n\n\n",destination_port);


	port = open_port(uart_dev); // ask open serial TODO : specify SERIAL port  & UDP adress + port via arguments
	if(port != -1)
	{
		 set_baudrate(port,uart_baudrate); // set baudrate 
		if(!open_connection(destination_address,destination_port))
		{
			pthread_create(&ThreadSer, NULL,SerialTask,NULL); //create a thread to get serial data and push it over UDP
			pthread_create(&ThreadUDP, NULL,UdpTask,NULL); // //create a thread to get UDP data and push it over serial
			
			pthread_mutex_lock(&conditionSer_done_locker);
			pthread_mutex_lock(&conditionUdp_done_locker);

			while(1) // ùain 
			{
				int n;
				FD_ZERO(&input);
				FD_SET(port, &input);
				FD_SET(sockfd, &input);
				max_fd = (sockfd > port ? sockfd : port) + 1;
				/*printf("port %d",port);
				printf("socket %d",sockfd);
				printf("max fd %d",max_fd);*/
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
					 
						FD_CLR(port, &input); // clear the bit for serial port file descriptor
						SerialProcess = true;
						printf("DATA on SERIAL \n"); 
						pthread_mutex_lock(&conditionSer_locker); // notify SerialThread to wake up and process data
						pthread_cond_signal(&conditionSer);
						pthread_mutex_unlock(&conditionSer_locker);
					 
						pthread_cond_wait(&conditionSer_done,&conditionSer_done_locker);//wait notification form serial thread work is done	
							
						
					}
					if (FD_ISSET(sockfd, &input))
					{
						FD_CLR(sockfd, &input);  // clear the bit for udp  socket  file descriptor
						printf("DATA on UDP \n"); 
						pthread_mutex_lock(&conditionUdp_locker); // notify UdpThread to wake up and process data
						pthread_cond_signal(&conditionUdp);
						pthread_mutex_unlock(&conditionUdp_locker);

						pthread_cond_wait(&conditionUdp_done,&conditionUdp_done_locker); //wait notification from udp thread work is done
						
					}
				
				

				
				}
				else
				{
				  fprintf(stdout,"TIMEOUT \n"); //we didn't get  any  data for 10 seconds 
				   
				  
				}
				
			}
			pthread_mutex_unlock(&conditionSer_done_locker);
			pthread_mutex_unlock(&conditionUdp_done_locker);
		}
	}
	
	
	
	
	return 0;
}

