// This file gets the present directory

#include <stdlib.h>

char PresentDir[1000];

char * getDir(char * CurrentDir, int Length){
	int i,j=0;
	char * HOME;
	HOME = (char * )malloc(sizeof(char)*100);
	HOME[0] = '~';
	// Checking if the current directory contains a substring that was the directory initially
	// If not return the directory
	char * Current = CurrentDir;
	for(i=0;i<Length;i++){
		if(CurrentDir[i]!=PresentDir[i])
			return CurrentDir;
	}
	
	// Else replace the substring with '~' and print the rest of directory
	while(CurrentDir[i]){
		HOME[j+1] = CurrentDir[i];
		i++,j++;
	}
	i=1;
	if(HOME[1]!='/') while (HOME[i]){HOME[i]='\0';i++;}
	
	return HOME;
}
