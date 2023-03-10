//command line parser for MyShell
//Giorgio Tatarelli
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include "shell_header.h"

int parse(char *line, char **envp, char abs[]){
	char delims[4] = " \t\n";//delimiters for strtok
	char *special[5] = {">", "<", ">>", "|", "&"};//special command characters

	//declares arrays to hold command line and individual commands
	char *commands[16];
	char *cmd1[16];
	int len1 = 0;
	char *cmd2[16];
	int len2 = 0;
	int current_cmd = 1;//keeps track of how many commands there are
	char *specChar = " ";//will store special command character
	
	int stdInSave = dup(0);//save stdin file descriptor
	int stdOutSave = dup(1);//save stout file descriptor
	int rdin = 0;
	int rdout = 0;	
	int append = 0;
	char rdinFile[32];
	char rdoutFile[32];
	int ampCount = 0;
	int hasPipe = 0;

	char *tok;
	int count = 0;
	tok = strtok(line, delims);//tokenizes input
	while(tok != NULL){	
		commands[count] = malloc(strlen(tok)*sizeof(char));
		strcpy(commands[count], tok);//stores each argument in commands[]
		
		tok = strtok(NULL, delims);

		count++;//keep count of number of arguments in command line
	}

	for(int i = 0; i < count; i++){//loops over each argument in command line
		for(int j = 0; j < 5; j++){//loops over special characters
			if(!strcmp(special[j],commands[i])){
				current_cmd = 2;//special character usually indicates new command
				specChar = calloc(strlen(special[j]), sizeof(char));

				strcpy(specChar, special[j]);//stores special character
			
				if(strcmp(specChar, "&")){//& is the only op that can go at the end
					if(i == count - 1){//if op is last arg, throw error
						perror("an error has occurred");
						return 1;
					}
				}
				if(i == 0){//if op is first arg, throw error
					perror("an error has occurred");
					return 1;
				}

				if(!strcmp(specChar,"<")){
					rdin = 1;
					strcpy(rdinFile, commands[i+1]);
				}
				if(!strcmp(specChar,">")){
					rdout = 1;
					strcpy(rdoutFile, commands[i+1]);
				}
				if(!strcmp(specChar,">>")){
					append = 1;
					rdout = 1;
					strcpy(rdoutFile, commands[i+1]);
				}
				if(!strcmp(specChar,"|")){
					hasPipe = 1;
				}
				if(!strcmp(specChar,"&")){
					ampCount++;
				}

				if((rdin || rdout) && hasPipe){
					perror("an error has occurred");
					return 1;
				}	
	
				i++;//goes to next arg
				break;
			}
		}
		if(current_cmd == 1){//if special charater not encountered
			cmd1[len1] = malloc(strlen(commands[i])*sizeof(char));
			strcpy(cmd1[len1], commands[i]);//stores first command in its own array

			len1++;//counts args in first cmd
		}
		else if(i < count){//after special character, starts filling second command arr
			cmd2[len2] = malloc(strlen(commands[i])*sizeof(char));
			strcpy(cmd2[len2], commands[i]);//stores second command in arr

			len2++;//counts args in second cmd
		}
	}
	//all special command characters check for several things:
	//that there are two commands and the length of these commands is > 0
	if(rdin){
		//below code adapted from Week 7 Lab slides 
		int new_fd = open(rdinFile, O_RDONLY);//open given file for reading
		if(new_fd == -1){
			perror("an error has occurred");
			return 1;
		}
		dup2(new_fd, 0);//replace stdin with file	
		close(new_fd);//close duplicate file descriptor
	}
	if(rdout && !append){
		//below code adapted from Week 7 Lab slides 
		//opens file for writing, creates it if it doesn't exist
		//0777 grants read/ write permissions
		//the rest is similar to above but replaces and restores stdout instead
		int new_fd = open(rdoutFile, O_WRONLY|O_CREAT|O_TRUNC,0777);
		dup2(new_fd, 1);	
		close(new_fd);
	}
	if(rdout && append){
		//below code adapted from Week 7 Lab slides 
		//O_APPEND means it appends to the specified file instead of overwriting
		int new_fd = open(rdoutFile, O_WRONLY|O_CREAT|O_APPEND,0777);
		dup2(new_fd, 1);	
		close(new_fd);

	}
	if(!strcmp(specChar,"|") && current_cmd == 2 &&  len1 > 0 && len2 > 0){
		//below code adapted from pipe and fork example code on Canvas
		int pip[2];//array to hold file descriptors
   		int pid;
 		
  	 	if(pipe(pip) < 0){//checks for pipe creation success
			perror("(1) an error has occurred");
			return 1;
		}

        	pid = fork();//creates child for first command
    
        	if(pid == 0){
       			close(1); //close stdin
        
      			dup2(pip[1], 1);//replace stdin with the write end of the pipe
        		close(pip[0]);//close the other end
			cmd1[len1] = NULL;//NULL terminates argument array for execvp
        		if(execvp(cmd1[0], cmd1) < 0){//if execvp returns, an error has occurred
				perror("(2) an error has occurred");
				return 1;
			}
        	} 
        	else { 
     			pid = fork(); //creates another child for the second command
     			if(pid == 0){ 
       		 		close(0); //close stdout
 		      		dup2(pip[0], 0);//replace stdout with read end of pipe 
 	           		close(pip[1]); //close the other end
   				
				cmd2[len2] = NULL; 
            			if(execvp(cmd2[0], cmd2) < 0){
     					perror("(3) an error has occurred");
        				return 1;
     				}
        	    	} 
 			else {
                		wait(&pid);//waits for one of the children
               		}
                }
    
  		close(pip[0]);//close ends of pipe
   		close(pip[1]);
     
		int wstatus;
 		waitpid(0,&wstatus,WUNTRACED);
 		waitpid(0,&wstatus,WUNTRACED);	
	}
	if(!strcmp(specChar,"&") && current_cmd == 2 && len1 > 0){
		int pid = fork();//fork so we can execute one command at the same time as parent
		if(pid == 0){
			cmd1[len1] = NULL;
			execvp(cmd1[0], cmd1);//executes command and terminates child
			perror("(4) an error has occurred");//this will not run unless execvp fails
			return 1;
		}
		else{
			if(ampCount == 2){
				pid = fork();
				if(pid == 0){
					cmd2[len2] = NULL;
					execvp(cmd2[0], cmd2);//executes command and terminates child
					perror("(4) an error has occurred");//this will not run unless execvp fails
					return 1;
				}
			}
		}
	}
	if(current_cmd != 2){//this is the case where we run a single command
		if(!builtIns(cmd1, len1, envp, abs)){
			runExternal(cmd1, len1);	
		}
	}
	
	if(rdin || rdout){
		if(!builtIns(cmd1, len1, envp, abs)){
			runExternal(cmd1, len1);	
		}
	}

	if(rdin){
		fflush(stdin);//clear buffer
		dup2(stdInSave, 0);//restore stdin
		close(stdInSave);//close duplicate
	}
	if(rdout){
		fflush(stdout);
		dup2(stdOutSave, 1);
		close(stdOutSave);
	}

	return 0;
}

int runExternal(char **cmd, int len){
	int pid = fork();//creates child
	if(pid == -1){//throws error on fork failure
		perror("an error has occurred");
		return 1;
	}
			
	if(pid == 0){//cild
		cmd[len] = NULL;//NULL terminate
		execvp(cmd[0], cmd);
		perror("an error has occurred");//error if execvp returns
		return 1;
	}
	else{//parent waits for child to complete
		waitpid(pid, NULL, 0);
	}

	return 0;
}

