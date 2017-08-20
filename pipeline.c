#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<errno.h>
#include<pwd.h>
#include<signal.h>
#include<sys/stat.h>

char input[64],output[64];
char *command[512];

int redirect_in(char *command[],int in)
{
    int u;
    for(u=0;command[u]!='\0';u++)
    {
        if(strcmp(command[u],"<")==0)
        {        
            command[u]=NULL;
            strcpy(input,command[u+1]);
            in=1;           
        }
    }
    return in;
}

int redirect_out(char *command[], int out)
{
    int v;
    for(v=0;command[v]!='\0';v++)
    {
        if(strcmp(command[v],">")==0)
        {      
            command[v]=NULL;
            strcpy(output,command[v+1]);
            out=1;
        }
    }
    return out;
}
void piping(char** argument)
{
    int p1[2], p2[2];
    int number = 0;

    pid_t pid;
    int end=0;
    int i=0,j,k=0,l;
    l=0;
    while (argument[l] != NULL)
    {
        if (strcmp(argument[l],"|") == 0)
            number++;
        l++;
    }
    number++;
    j=0;
    while(argument[j]!=NULL)
    {
        k = 0;
        while (strcmp(argument[j],"|") != 0)
        {
            command[k] = argument[j];
            j++;	
            if (argument[j] == NULL)
            {
                k++;
                break;
            }
            k++;
        }
        command[k] = NULL;

        if (i%2 != 0)
            pipe(p1); 
        else
            pipe(p2); 

        int in=0;
        int out=0;

        pid=fork();

        if(pid==-1)
        {
            printf("Child process could not be created\n");
            return;
        }
        else if(pid==0)
        {
            if (i==0)
            {
                int u;
                in=0;
                /*for(u=0;command[u]!='\0';u++)
                {
                    if(strcmp(command[u],"<")==0)
                    {        
                        command[u]=NULL;
                        strcpy(input,command[u+1]);
                        in=1;           
                    }
                }*/
                in = redirect_in(command,in);
                if(in)
                {   int fd0;
                    if ((fd0 = open(input, O_RDONLY, 0)) < 0) 
                    {
                        perror("Couldn't open input file");
                        exit(0);
                    }  
                    dup2(fd0,0);
                    close(fd0);
                }
                dup2(p2[1], 1);
            }
            else if (i==number-1)
            {
               /* for(v=0;command[v]!='\0';v++)
                {
                    if(strcmp(command[v],">")==0)
                    {      
                        command[v]=NULL;
                        strcpy(output,command[v+1]);
                        out=1;
                    }
                }*/
                out = redirect_out(command, out);
                if (out)
                {
                    int fd1 ;
                    if ((fd1 = creat(output , 0755)) < 0) {
                        perror("Couldn't open the output file");
                        exit(0);
                    }           

                    dup2(fd1, 1);
                    close(fd1);
                }
                if (number%2!=0)
                    dup2(p1[0],0);
                else
                    dup2(p2[0],0);
            }
            else
            { 
                if (i%2!=0)
                {
                    dup2(p2[0],0); 
                    dup2(p1[1],1);
                }
                else
                { 
                    dup2(p1[0],0); 
                    dup2(p2[1],1);					
                } 
            }

            if (execvp(command[0],command) < 0)
            {
                printf("Command could not be executed\n");
            }		
        }
        else
        {
            if (i==0)
                close(p2[1]);
            else if (i==number-1)
            {
                if (number%2 != 0)
                    close(p1[0]);
                else
                    close(p2[0]);
            }
            else
            {
                if (i%2==1)
                {					
                    close(p2[0]);
                    close(p1[1]);
                }
                else
                {					
                    close(p1[0]);
                    close(p2[1]);
                }
            }

            waitpid(pid,NULL,0);
        }
        j++;		
        i++;	
    }
}

