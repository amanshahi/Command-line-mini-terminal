#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define PATH 100

int pinfo(char ** parameters, char * PresentDir){
    int pid; char status; char processName[20];
    int Memory;
    // This creates a string with the given path
    char ProcessPath[PATH];
    strcpy(ProcessPath, "/proc/");

    // If parameters[1] is not NULL use pid of that else use pid of self
    if(parameters[1]) strcat(ProcessPath, parameters[1]);
    else strcat(ProcessPath, "self");

    char status_Path[PATH];
    strcpy(status_Path, ProcessPath); strcat(status_Path, "/stat");

    errno = 0;
    FILE * stat = fopen(status_Path, "rw");
    if(errno){
        printf("Some error occurred in reading the file %s: %s\n", status_Path, strerror(errno));
        return 0;
    }
    // Read the process Id and status
    fscanf(stat, "%d", &pid); fscanf(stat, "%s", processName); fscanf(stat, " %c", &status);

    printf("pid of process %d\n",pid);
    printf("Process Status (R/NR): %c\n", status);
//    fclose(stat);
    
    // read number of virtual memory pages
    errno = 0;
    strcpy(status_Path, ProcessPath); strcat(status_Path, "/statm");
    FILE * mem = fopen(status_Path, "r");
    if(errno){
        printf("Some error occurred  in %s: %s\n", status_Path, strerror(errno));
        return 0;
    }
    fscanf(mem, "%d", &Memory);
    printf("Memory (M): %d\n", Memory);
//    fclose(mem);
    
    // This array stores the path of the executable 
    char ExecutablePath[PATH];
    int i = 0;

    // Initialize with NULL
    while(i<PATH) ExecutablePath[i++] = '\0';
    strcpy(status_Path, ProcessPath); strcat(status_Path, "/exe");
    
    errno = 0;
    // Note : readlink doesn't add null terminating byte
    readlink(status_Path, ExecutablePath, sizeof(ExecutablePath));
    if(errno){
        printf("Some error occurred in  %s: %s\n", status_Path, strerror(errno));
        return 0;
    }

    // If found the homepath (PresentDir) replace it with ~ and then add the remaining of the path
    int sameChars = 0, baseLength = strlen(PresentDir);
    for(sameChars = 0; sameChars < baseLength; sameChars++)
        if(PresentDir[sameChars] != ExecutablePath[sameChars]) break;;

    // Finally getting the relative path of the executable    
    char relative_Path[PATH];
    if(sameChars == baseLength) {
        // First part of path is common
        relative_Path[0] = '~'; relative_Path[1] = '\0';
        strcat(relative_Path, (const char *)&ExecutablePath[baseLength]);
    }
    else{
        strcpy(relative_Path, ExecutablePath);
        relative_Path[strlen(ExecutablePath)] = '\0';
    }

    // Clear ExecutablePath of the stored exe path
    i = 0;
    while(ExecutablePath[i]) ExecutablePath[i++] = '\0';

    printf("Executable Path: %s\n", relative_Path);
    return 0;
}
