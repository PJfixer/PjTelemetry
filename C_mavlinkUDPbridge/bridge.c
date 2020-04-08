
#include "bridge.h"


pthread_mutex_t serial_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t udp_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t conditionSer_locker = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conditionSer = PTHREAD_COND_INITIALIZER ; 

pthread_mutex_t conditionUdp_locker = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conditionUdp = PTHREAD_COND_INITIALIZER ;

pthread_mutex_t conditionSer_done_locker = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conditionSer_done = PTHREAD_COND_INITIALIZER ; 

pthread_mutex_t conditionUdp_done_locker = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conditionUdp_done = PTHREAD_COND_INITIALIZER ;





 volatile bool SerialProcess;
 volatile bool UdpProcess;
 extern int port ;
 extern int sockfd; 
 extern struct sockaddr_in servaddr;

void *SerialTask(void *arg)
{
	printf("hello from serial Thread !! \n");
	char * SerBuf;
	pthread_mutex_lock(&conditionSer_locker);
	while(1)
	{
	
	pthread_cond_wait(&conditionSer,&conditionSer_locker);//wait notification form main 
	
	pthread_mutex_lock(&serial_mutex); // lock serial port 
	
	
	int d = read(port,&SerBuf,512); // here we read the data and store in Serbuf
   
    fprintf(stdout,"read %d bytes from serial\n",d);  // porint the number of bytes we effectively read
    
	pthread_mutex_unlock(&serial_mutex); // unlock serial port 
	
	
	pthread_mutex_lock(&udp_mutex); // lock udp socket 
	

	sendto(sockfd,&SerBuf,d,MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr)); // here we push the data
				
	
	pthread_mutex_unlock(&udp_mutex); // unlock udp socket 
	
	pthread_mutex_lock(&conditionSer_done_locker); // notify main job is done 
	pthread_cond_signal(&conditionSer_done);
	pthread_mutex_unlock(&conditionSer_done_locker);
	
	}
	pthread_mutex_unlock(&conditionSer_locker);
	pthread_exit(NULL);
}


void *UdpTask(void *arg)
{
	printf("hello from UDP Thread !! \n");
	
	int d, len;
	char buffer[512]; 
	pthread_mutex_lock(&conditionUdp_locker);
	while(1)
	{
	
	pthread_cond_wait(&conditionUdp,&conditionUdp_locker); //wait notification from main 
	
	pthread_mutex_lock(&udp_mutex); // lock udp socket 
	// here we read the data
	d = recvfrom(sockfd,&buffer, 512,MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
	if(d == -1)
	{
		printf("fail: %s",strerror(errno) );
	}
	buffer[d] = '\0';  // in order  to create proper str
	pthread_mutex_unlock(&udp_mutex); // unlock udp socket 
	fprintf(stdout,"read %d bytes from UDP\n",d); 
	
	pthread_mutex_lock(&serial_mutex); // lock serial port
	// here we psuh the data
	write(port,(char *)buffer,d); 
	pthread_mutex_unlock(&serial_mutex); // unlock serial port

	pthread_mutex_lock(&conditionUdp_done_locker); // notify main job is done 
	pthread_cond_signal(&conditionUdp_done);
	pthread_mutex_unlock(&conditionUdp_done_locker);

	}
	pthread_mutex_unlock(&conditionUdp_locker);
}
