#include "unp.h"
#include <time.h>

void *timeServ(void *args)
{
	time_t ticks = time(NULL);
	char buff[MAXLINE];
	int sockfd = (int) args;
	snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
	Writen(sockfd, buff, strlen(buff));
	close(sockfd);
	return (NULL);
}
