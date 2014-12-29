/*
File: client.c
Action:
1. Echo client
2. Day Time Client
3. Quit
*/
#include<stdio.h>
#include<unp.h>
#include "msg.h"

#ifndef SIZE
#define SIZE 1024
#endif

// Signal "SIGCHLD" handler.
void sig_child(int signo)
{
	pid_t pid;
	int stat;
	pid = wait(&stat);
	Fputs(CLIEND,stdout);
	return;
}

char *getChoice(int ch)
{
	if(ch == 1)
		return "./echo_cli";
	else if(ch == 2)
		return "./timeserver";
	else 
		return;
}

void usage()
{
	err_quit("\nUsage: ./client <IP Adress/Hostname>\n");
}

void readArguments()
{
	printf("\n1. Echo Client\n");
	printf("2. Day Time Client\n");
	printf("3. Quit\n");
	printf("Enter Choice: ");
}

int checkString(char *args)
{
	int ret=0;
	if((args[0] >= 65 && args[0] <= 90) || (args[0] >= 97 && args[0] <= 122)) ret = -1;
	return ret;
}

int main(int argc, char **argv)
{
	int ch, nread;
	int pfd[2];
	char *ip;
	int sockfd, connfd;
	struct hostent *hostp;
	struct in_addr inaddr;
	struct in_addr **ptr;
	char param[2];
	pid_t pid;
	char buf[SIZE];
	if(argc != 2)
	{
		printf("Too few arguments.!");
		usage();
	}
	
	if( (hostp = gethostbyname(argv[1])) == NULL) {
		if(inet_aton(argv[1], &inaddr) == 0) 
		{
			err_quit("hostname error.!\n");
		}
	} else {
		ptr = (struct in_addr **)hostp->h_addr_list;
		memcpy(&inaddr,*ptr, sizeof(struct in_addr));
	}
	ip = inet_ntoa(inaddr);
	// printf("IP: %s\n",ip);
	readArguments();
	scanf("%d",&ch);
	Signal(SIGCHLD,sig_child);
	
	do
	{		
		// Quit check.!
		if(ch == 3) 
			exit (0);
		else if(ch < 0 || ch > 3)
			err_quit("\nInvalid choice.!\nGracefully exiting Client.!\n");
		if (pipe(pfd) == -1)
		{
			perror("Pipe failed.!\n");
			exit(0);
		}
		
		if((pid = fork()) < 0)
			exit(2);
		
		if(pid == 0)
		{
			// Child
			Close(pfd[0]); // Blocking Read for child.
			sprintf(param,"%d", pfd[1]);
			execlp("xterm","xterm","-e",getChoice(ch),ip,param,buf,(char *)0);
			exit(0); // This is required, as the process will not be killed by itself.
		} else 
		{
			// Parent 
			Close(pfd[1]); // Blocking write for parent.
			while ((nread = Read(pfd[0], buf, SIZE)) != 0)
				printf("[Parent] %s\n", buf);
			Close(pfd[0]);
			readArguments();
			scanf("%d",&ch);
		}
	} while(ch < 3 && ch > 0);
	exit(0);
}
