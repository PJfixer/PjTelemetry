
    #include <sys/types.h> /*  linux sys types */
	#include <sys/time.h> /* linux sys time struct*/
	#include <sys/select.h> /* select fd */
	#include "udp.h"
    int main()
    {
		extern int sockfd; 
		extern struct sockaddr_in servaddr;
		char *hello = "met pllus de jeu sur yoyo"; 
		int n;
		printf("UDP client with select \n");
		if(!open_connection("127.0.0.1",8000))
		{
			int            max_fd;
			fd_set         input;
			struct timeval timeout;
			 char buffer[1024]; 
			
			while(1)
			{
			   /* Initialize the timeout structure */
				FD_ZERO(&input);
				FD_SET(sockfd, &input);
				max_fd = (sockfd+1);
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
				  if (FD_ISSET(sockfd, &input))
					{
						fprintf(stdout,"\n"); 
						int d, len; 
						d = recvfrom(sockfd, (char *)buffer, 1024,MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
						buffer[d] = '\0'; 
						printf("Server : %s\n", buffer); 
					 
					 
					}
				 
				}
				else
				{
				  fprintf(stdout,"SENDING :  \n");
				  printf("%s",hello);
				  sendto(sockfd, (const char *)hello, strlen(hello), 
				MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
				sizeof(servaddr)); 
				   
				}

			}
		}
		
		return 0;
    }
    
    
