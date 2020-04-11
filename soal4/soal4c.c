#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main() 
{ 
	pid_t pid;
    int fd[2];
    int status;
    if (pipe(fd)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
    pid = fork();

    if (pid < 0 ){
        fprintf(stderr, "fork Failed" ); 
		return 1;
    }else if(pid == 0){
        close(1);
        dup(fd[1]);
        close(fd[0]);
        execlp("ls","ls",NULL);
    }else{
        wait(&status);
        close(0);
        dup(fd[0]);
        close(fd[1]);
        execlp("wc","wc","-l",NULL);
    }
} 
