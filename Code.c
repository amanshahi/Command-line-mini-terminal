// This is the main file

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "print.h"
#include "getDir.h"
#include "parseCmd.h"
#include "executeCd.h"
#include "executeCmd.h"
#include "executeEcho.h"
#include "exePinfo.h"

#define COMMAND 100

char Command[1000];
char CurrentDir[1000];
char CommandStore[1000];
char LoginUser[20];
char * SplitCommands;
char Command_run[100][100];
char PresentDir[1000];
char PresentDirectory[1000];

int main(){
	// The commands entered is stored in cmd
	char command[COMMAND + 1];

	// The params breaks it into seperate commands
	char * parameters[COMMAND + 1], *CurrentFinal;

	int cmdCount = 0,i=0,temp=0,Length;
	
	// Getting the present direcctory
	getcwd(PresentDir, sizeof(PresentDir));
	Length = strlen(PresentDir);
	strcpy(PresentDirectory,PresentDir);
	CurrentFinal = (char *)malloc(sizeof(char)*100);
	
	while(1) {
		temp=0;
		// Getting the present directory
		getcwd(CurrentDir, sizeof(CurrentDir));
		
		// Checking if our new root is a substring of pwd
		CurrentFinal = getDir(CurrentDir,Length);
		
		// Returns the name of user logged in
		const char * LoginUser = getenv("USER");
		
		// Printing the shell
		print(LoginUser,CurrentFinal);

		// Scanning the command
		if(fgets(command, sizeof(command), stdin) == NULL) break;
		if(command[strlen(command)-1] == '\n') {
			command[strlen(command)-1] = '\0';
		}

		//If command = exit, then exit
		if(strcmp(command,"exit") == 0) break;

		// Else split the commands based on ';'
		SplitCommands = strtok(command,";");

		// Store the commands in an array
		while(SplitCommands){
			strcpy(Command_run[temp],SplitCommands);
			SplitCommands = strtok(NULL,";");
			temp++;
		}

		// Execute all the commands
		for(i=0;i<temp;i++){
			breakCommand(Command_run[i], parameters);

			// Execute exit
			if(strcmp(parameters[0], "exit") == 0) break;

			// Execute pinfo
			else if(strcmp(parameters[0],"pinfo") == 0){
				pinfo(parameters,PresentDirectory);
			}

			// Execute pwd
			else if(strcmp(parameters[0],"pwd") == 0){
				printf("%s\n",CurrentFinal);
			}

			// Execute cd
			else if(strcmp(parameters[0],"cd") == 0){
				executeCD(parameters);
			}

			// Execute echo
			else if(strcmp(Command_run[i],"echo") == 0){
				executeEcho(parameters);
			}
			else if(executeCommand(parameters) == 0) break;
		}
	}
	return 0;
}
