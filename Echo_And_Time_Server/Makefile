# This is a sample Makefile which compiles source files named:
# - tcpechotimeserv.c
# - tcpechotimecli.c
# - time_cli.c
# - echo_cli.c
# and creating executables: "server", "client", "time_cli"
# and "echo_cli", respectively.
#
# It uses various standard libraries, and the copy of Stevens'
# library "libunp.a" in ~cse533/Stevens/unpv13e_solaris2.10 .
#
# It also picks up the thread-safe version of "readline.c"
# from Stevens' directory "threads" and uses it when building
# the executable "server".
#
# It is set up, for illustrative purposes, to enable you to use
# the Stevens code in the ~cse533/Stevens/unpv13e_solaris2.10/lib
# subdirectory (where, for example, the file "unp.h" is located)
# without your needing to maintain your own, local copies of that
# code, and without your needing to include such code in the
# submissions of your assignments.
#
# Modify it as needed, and include it with your submission.

CC = gcc

LIBS = -lresolv -lsocket -lnsl -lpthread\
	/home/courses/cse533/Stevens/unpv13e_solaris2.10/libunp.a\
	
FLAGS = -g -O2

CFLAGS = ${FLAGS} -I/home/courses/cse533/Stevens/unpv13e_solaris2.10/lib \
		 -I/home/stufs1/ssrinath/Raj/Lab01/include

all: client server echo_cli time_cli 

debug: CC = gcc -ggdb
debug: CFLAGS += -DDEBUG
debug: client server echo_cli time_cli 

time_cli: timeClient
	${CC} ${FLAGS} -o time_cli Client/time_cli.o ${LIBS}
timeClient: Client/time_cli.c
	cd Client; \
	${CC} ${CFLAGS} -c time_cli.c


echo_cli: echoClient
	${CC} ${FLAGS} -o echo_cli Client/echo_cli.o Client/echo_fun.o ${LIBS}
echoClient: Client/echo_cli.c echoFun
	cd Client; \
	${CC} ${CFLAGS} -c echo_cli.c 
echoFun: Client/echo_fun.c
	cd Client; \
	${CC} ${CFLAGS} -c echo_fun.c
	

# server uses the thread-safe version of readline.c

server: echotimeServ readLine # timeserv
	${CC} ${FLAGS} -o server Server/tcpechotimesrv.o Server/readline.o ${LIBS}
echotimeServ: Server/tcpechotimesrv.c
	cd Server; \
	${CC} ${CFLAGS} -c tcpechotimesrv.c
timeserv: Server/timeserv.c
	cd Server; \
	${CC} ${CFLAGS} -c timeserv.c


client: tcpechotimecli
	${CC} ${FLAGS} -o client Client/tcpechotimecli.o ${LIBS}
tcpechotimecli: Client/tcpechotimecli.c
	cd Client; \
	${CC} ${CFLAGS} -c tcpechotimecli.c


# pick up the thread-safe version of readline.c from directory "threads"

readLine: /home/courses/cse533/Stevens/unpv13e_solaris2.10/threads/readline.c
	cd Server; \
	${CC} ${CFLAGS} -c /home/courses/cse533/Stevens/unpv13e_solaris2.10/threads/readline.c

clean:
	rm -f echo_cli Client/echo_cli.o client Client/tcpechotimecli.o time_cli Client/time_cli.o server Server/tcpechotimesrv.o Server/readline.o
