// This file parses the command into array of commands

#include <string.h>
#define COMMAND 100

void breakCommand(char* cmd, char** params){
	// USing strtok to break commands based on " "
	char * Split_temp = strtok(cmd," ");
	for(int i = 0; i < COMMAND; i++) {
		params[i] = Split_temp;
		Split_temp = strtok(NULL," ");
		if(params[i] == NULL) break;
	}
}