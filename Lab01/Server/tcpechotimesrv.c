#include "unp.h"
#include <stdio.h>
#include "msg.h"

static void *echoSrv (void *args);
void str_echo(int fd);

int main(int argc, char **argv)
{
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in servaddr, cliaddr;
	pthread_t tid;
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any available Interface to connect with the client.!
	servaddr.sin_port = htons(SERV_PORT);
	
	Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
	Listen(listenfd,LISTENQ);
	
	for(;;)
	{
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
		// Multi-Threaded Application to handle clients.
		Pthread_create(&tid,NULL,&echoSrv,(void *)connfd);
		// Close(connfd);
	}
	exit(0);	
}

static void *echoSrv (void *args)
{
	char send[MAXLINE];
	Pthread_detach(pthread_self());
	Fputs(ECHOESTD, stdout);
	str_echo((int) args);
	Close((int) args);
	return (NULL);
}

void str_echo(int fd)
{
	ssize_t n;
	char buf[MAXLINE];
   again:
	while((n = read(fd,buf,MAXLINE)) > 0)
		Writen(fd, buf, n);
	if(n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}
