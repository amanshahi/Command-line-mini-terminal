// This file executes the echo command

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

char executeEcho(char ** params){
	int i=1,flag1 = 0,flag2 = 0,flag3 = 0, last = 0;
	// Checking "" in the argument and simply printing the string inside it
	while(params[i]){
		if(i == 1 && params[i][0] == '\"'){
			flag1=1;
		}
		if(params[i][strlen(params[i])-1]!='\"'){
			i++;
		}
		else if(params[i][strlen(params[i]) - 1] == '\"'){
			flag2 = 1;
			if(params[i+1]) flag3 = 1;
			last = i;
			break;
		}
	}
	if(flag1==0 && flag2==0 && flag3==0){
		i=1;
		printf(">");
		while(params[i]){
			printf("%s ",params[i]);
			i++;
		}
		printf("\n");
	}
	else if(flag3 == 0 && flag1 == 1 && flag2 == 1){
		i=1;
		int j=1;
		params[1][0]='>';
		params[last][strlen(params[last])-1] = ' ';
		while(params[i]){
			printf("%s ",params[i]);
			i++;
		}
		printf("\n");
	}
	// Cheking error if found only one " etc	
	else if(flag1 == 1 || flag2 == 1){
		printf("Error occurred in command : echo\n");
	}
	return 0;
}
