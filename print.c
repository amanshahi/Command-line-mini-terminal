// This file prints the name of the shell

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int print(const char * LoginUser, char * CurrentDir){
	// Arguments passed are current directory and the username
	char hostName[100];
	gethostname(hostName,sizeof(hostName));
	printf("\e[1m\e[92m%s\e[1m@\e[92m%s: \e[94m%s $ ",LoginUser,hostName,CurrentDir);
	return 0;
}