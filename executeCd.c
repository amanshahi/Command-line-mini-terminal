// This executes the cd command

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

char PresentDir[100];

int executeCD(char ** params){
	int err;
	// Checking args[1]
	// If found go to that directory
	if(params[1]){
		if(strcmp(params[1],"~") != 0){
			err = chdir(params[1]);
			if(err == -1) printf("Error occurred. Check the name of directory.\n");
		}
		else{
			chdir(PresentDir);
		}
	}
	// Else to root defined
	else{
		chdir(PresentDir);
	}
	return 0;
}
