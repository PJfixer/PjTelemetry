
#include "bridge.h"


pthread_mutex_t serial_mutex;
pthread_mutex_t udp_mutex;
 volatile bool SerialProcess;
 volatile bool UdpProcess;


void SerialTask(void)
{
	SerialProcess = true;
	pthread_mutex_lock(&serial_mutex); // lock serial port 
	// here we read the data
	pthread_mutex_unlock(&serial_mutex); // unlock serial port 
	
	pthread_mutex_lock(&udp_mutex); // lock udp socket 
	// here we push the data
	pthread_mutex_unlock(&udp_mutex); // unlock udp socket 
	SerialProcess = false;
}


void UdpTask(void)
{
	UdpProcess = true ;
	pthread_mutex_lock(&udp_mutex); // lock udp socket 
	// here we read the data
	pthread_mutex_unlock(&udp_mutex); // unlock udp socket 
	
	pthread_mutex_lock(&serial_mutex); // lock serial port 
	// here we psuh the data
	pthread_mutex_unlock(&serial_mutex); // unlock serial port
	UdpProcess = false;
}
