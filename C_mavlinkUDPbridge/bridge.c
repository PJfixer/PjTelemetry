
#include "bridge.h"


pthread_mutex_t serial_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t udp_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t conditionSer_locker = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conditionSer = PTHREAD_COND_INITIALIZER ;

pthread_mutex_t conditionUdp_locker = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conditionUdp = PTHREAD_COND_INITIALIZER ;

 volatile bool SerialProcess;
 volatile bool UdpProcess;
 extern int port ;
 extern int sockfd; 
 extern struct sockaddr_in servaddr;

void *SerialTask(void *arg)
{
	printf("hello from serial Thread !! \n");
	char * SerBuf;
	while(1)
	{
	pthread_mutex_lock(&conditionSer_locker);
	pthread_cond_wait(&conditionSer,&conditionSer_locker);
	
	pthread_mutex_lock(&serial_mutex); // lock serial port 
	// here we read the data
	
	int d = read(port,&SerBuf,512);
   
	fprintf(stdout,"read %d bytes from serial\n",d); 
	pthread_mutex_unlock(&serial_mutex); // unlock serial port 
	
	
	pthread_mutex_lock(&udp_mutex); // lock udp socket 
	// here we push the data

	sendto(sockfd,&SerBuf,d, 
				MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
				sizeof(servaddr)); 
				
	
	pthread_mutex_unlock(&udp_mutex); // unlock udp socket 
	SerialProcess = false;
	pthread_mutex_unlock(&conditionSer_locker);
	
	}
	pthread_exit(NULL);
}


void *UdpTask(void *arg)
{
	printf("hello from UDP Thread !! \n");
	
	int d, len;
	char buffer[512]; 
	while(1)
	{
	pthread_mutex_lock(&conditionUdp_locker);
	pthread_cond_wait(&conditionUdp,&conditionUdp_locker);
	
	pthread_mutex_lock(&udp_mutex); // lock udp socket 
	// here we read the data
	d = recvfrom(sockfd,&buffer, 512,MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
	printf("fail: %s",strerror(errno) );
	buffer[d] = '\0'; 
	pthread_mutex_unlock(&udp_mutex); // unlock udp socket 
	fprintf(stdout,"read %d bytes from UDP\n",d); 
	
	pthread_mutex_lock(&serial_mutex); // lock serial port
	// here we psuh the data
	write(port,(char *)buffer,d); 
	pthread_mutex_unlock(&serial_mutex); // unlock serial port
	UdpProcess = false;
	pthread_mutex_unlock(&conditionUdp_locker);
	}
}