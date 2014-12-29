#include "unp.h"
#include "msg.h"

void echo_fun(FILE *fp, int sockfd)
{
	char send[MAXLINE], recv[MAXLINE];
	Fputs(INPUT, stdout);
	while(Fgets(send,MAXLINE,fp) != NULL)
	{
		Writen(sockfd,send,strlen(send));
		Fputs(SERVREPLY, stdout);
		if(Readline(sockfd,recv,MAXLINE) == 0)
			err_quit("error");		
		Fputs(recv, stdout);	
		Fputs(NEWLINE, stdout);
		Fputs(INPUT,stdout);
	}
}
