#include "unp.h"
#include "msg.h"

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	// if(argc != 2) // Sending 2 more parameters through exec.!
		// err_quit("Usage: echo_cli <IPAddress>\n");
	int pfd = atoi((char *)argv[2]);
	char *buf = argv[3];
	
	sockfd = Socket(AF_INET, SOCK_STREAM,0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET,argv[1], &servaddr.sin_addr);
	
	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	// printf("back to echo client PFD: %d \n",pfd);
	sprintf(buf,"%s",CLISTART);
	Writen(pfd,buf,strlen(buf)+1);	
	
	echo_fun(stdin, sockfd);
	
	sprintf(buf,"%s",CLIEND);
	Writen(pfd,buf,strlen(buf)+1);	
	
	exit(0);
}
