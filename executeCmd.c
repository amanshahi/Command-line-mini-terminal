// This file executes all the commands

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int executeCommand(char ** params){
	int flag = 0,i=0,temp;
	// Checking if the command contains any '&' to run it on background
	for(i=0;;i++){
		if(!params[i]) break;
	}
	temp = i;
	// If yes making the and Null and executing it on background
	if(params[temp-1]!=NULL){
		if(strcmp(params[temp-1],"&") == 0){
			params[temp-1] = '\0';
			flag = 1;
		}
	}
	// Checking fork (Invalid argument)
	pid_t pid = fork();
	if (pid == -1) {
		char* error = strerror(errno);
		printf("Error in fork: %s\n", error);
		return 1;
	}
	// Checking on the child process
	else if (pid == 0) {
		execvp(params[0], params);  
		char* error = strerror(errno);
		printf("Error in shell: %s: %s\n", params[0], error);
		return 0;
	}
	// Executing parent process 
	else {
		int childStatus;
		if(flag == 0){
				waitpid(pid, &childStatus, 0);
		}
		return 1;
	}
}
