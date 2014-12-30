#include "unp.h"
#include "msg.h"

void time_fun(int sockfd, int pfd, char *buf)
{
	char recv[MAXLINE];
	int n;
	while((n = Readline(sockfd,recv,MAXLINE)) > 0) {
		// err_quit("error");
		// Readline(sockfd,recv,MAXLINE);	
		Fputs(recv, stdout);	
		Fputs(NEWLINE, stdout);
	}
	if(n == 0) {
		sprintf(buf,"%s",UNEXPCRASH);
		Writen(pfd,buf,strlen(buf)+1);
	}	
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	int pfd = atoi((char *)argv[2]);
	char *buf = argv[3];
	
	sockfd = Socket(AF_INET, SOCK_STREAM,0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(TIME_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	// printf("back to echo client PFD: %d \n",pfd);
	sprintf(buf,"%s",TCLISTART);
	Writen(pfd,buf,strlen(buf)+1);	
	
	time_fun(sockfd, pfd, buf);
	
	sprintf(buf,"%s",TCLIEND);
	Writen(pfd,buf,strlen(buf)+1);
	
	exit(0);
}
