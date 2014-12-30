#include "unp.h"
#include <stdio.h>
#include <time.h>
#include "msg.h"
#include <fcntl.h>

static void *echoSrv (void *args);
void str_echo(int fd);
static void *timeServ(void *args);
void timeTick(int sockfd);

int main(int argc, char **argv)
{
	int listenfd, listenfd1, connfd, connfd1;
	pid_t childpid;
	socklen_t clilen, clilen1;
	struct sockaddr_in servaddr, servaddr1, cliaddr, cliaddr1;
	pthread_t tid, tid1;
	int maxfd, n;
	int option = 1, flags, flags1;
	fd_set selset;
	
	FD_ZERO(&selset);
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	listenfd1 = Socket(AF_INET, SOCK_STREAM, 0);
	
	// Setting Socket options.
	// This is for Reusing the bind address.!
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&option, (socklen_t)sizeof(option));
	setsockopt(listenfd1,SOL_SOCKET,SO_REUSEADDR,(const void *)&option, (socklen_t)sizeof(option));
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any available Interface to connect with the client.!
	servaddr.sin_port = htons(ECHO_PORT);
	
	bzero(&servaddr1, sizeof(servaddr1));
	servaddr1.sin_family = AF_INET;
	servaddr1.sin_addr.s_addr = htonl(INADDR_ANY); // Any available Interface to connect with the client.!
	servaddr1.sin_port = htons(TIME_PORT);
	
	Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
	Bind(listenfd1, (SA *)&servaddr1, sizeof(servaddr1)); 
	
	// Setting the listening sockets as non-blocking.
	if((flags = fcntl(listenfd, F_GETFL, 0)) < 0)
		err_sys(EFGETFL);
	if((flags1 = fcntl(listenfd1, F_GETFL, 0)) < 0)
		err_sys(EFGETFL);
	flags |= O_NONBLOCK;
	flags1 |= O_NONBLOCK;
	if(fcntl(listenfd, F_SETFL, flags)< 0)
		err_sys(EFSETFL);
	if(fcntl(listenfd1, F_SETFL, flags1)< 0)
		err_sys(EFSETFL);
	
	Listen(listenfd,LISTENQ);
	Listen(listenfd1,LISTENQ);
	
	maxfd=(listenfd > listenfd1)?listenfd:listenfd1;
	
	for(;;)
	{
		FD_SET(listenfd,&selset);
		FD_SET(listenfd1,&selset);
		
		n = Select(maxfd+1,&selset,NULL,NULL,NULL);
		
		if(FD_ISSET(listenfd,&selset)) {
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
			
			// Setting Accept connection as Blocking.!
			if((flags = fcntl(connfd, F_GETFL, 0)) < 0)
				err_sys(EFGETFL);
			flags &= ~O_NONBLOCK;
			if(fcntl(connfd, F_SETFL, flags)< 0)
				err_sys(EFSETFL);
			
			Pthread_create(&tid,NULL,&echoSrv,(void *)connfd);
			// FD_SET(connfd,&selset);
			// if(connfd > maxfd) maxfd = connfd;
			// if(--n <= 0) continue;
		}
		if(FD_ISSET(listenfd1,&selset)) {
			clilen1 = sizeof(cliaddr1);
			connfd1 = Accept(listenfd1, (SA *)&cliaddr1, &clilen1);
			// Setting Accept connection as Blocking.!
			if((flags1 = fcntl(connfd1, F_GETFL, 0)) < 0)
				err_sys(EFGETFL);
			flags1 &= ~O_NONBLOCK;
			if(fcntl(connfd1, F_SETFL, flags1)< 0)
				err_sys(EFSETFL);
			
			Pthread_create(&tid1,NULL,&timeServ,(void *)connfd1);
			// FD_SET(connfd1,&selset);
			// if(connfd1 > maxfd) maxfd = connfd1;
			// if(--n <= 0) continue;
		}
		
		// Multi-Threaded Application to handle clients.
		// if(FD_ISSET(connfd,&selset)) {
			// Fputs("echo hi darling",stdout);
			// FD_CLR(connfd,&selset);
			// Pthread_create(&tid,NULL,&echoSrv,(void *)connfd);
			// if(--n <= 0) continue;
				// Fputs(TCREATFAIL,stdout);
		// }
		// if(FD_ISSET(connfd1,&selset)) {
			// Fputs("hi darling",stdout);
			// Pthread_create(&tid1,NULL,&timeServ,(void *)connfd1);
			// if(--n <= 0) continue;
				// Fputs(TCREATFAIL,stdout);
		// }
		// Close(connfd1);
	}
	exit(0);
}

static void *echoSrv (void *args)
{
	Pthread_detach(pthread_self());
	Fputs(ECHOESTD, stdout);
	str_echo((int) args);
	// FD_CLR((int) args, (fd_set *)&selset);
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
	else if (n == 0) // read returns 0 when the connection is gone.
		Fputs(THRDEND,stdout);
}

static void *timeServ(void *args)
{	
	Pthread_detach(pthread_self());
	Fputs(TIMESTD, stdout);
	timeTick((int) args);
	Close((int) args);
	return (NULL);
}

void timeTick(int sockfd)
{
	char buff[MAXLINE];
	time_t ticks;
	int n;
	struct timeval timeout;
	fd_set timeSet;
	bzero(&timeout,sizeof(timeout));
	again:
		FD_SET(sockfd,&timeSet);
		Select(sockfd+1, &timeSet, NULL, NULL, &timeout);
		if(FD_ISSET(sockfd,&timeSet)) {
			if(read(sockfd,buff,MAXLINE) == 0)
				Fputs(THRDEND,stdout);
		} else {
			ticks = time(NULL);
			sprintf(buff, "%.24s\r\n", ctime(&ticks));
			// n = writen(sockfd, buff, strlen(buff));
			Writen(sockfd, buff, strlen(buff));
			// if(n == -1) {
				// Fputs(THRDEND,stdout);
				// return;
			// }	
			// sleep(5);
			timeout.tv_sec = TIMEOUT;
			goto again;
		}	
		// writen returns -1 on connection refused/closed at client end.
		// if(n < 0 && (errno == EPIPE || errno == 0)) {
			// goto again;
			// Fputs(THRDEND,stdout);
		// } else {
			// sleep(5);
			// goto again;
		// }	
}
