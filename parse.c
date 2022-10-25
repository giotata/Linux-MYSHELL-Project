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
	char delims[4] = " \t\n";
	char *special[5] = {">", "<", ">>", "|", "&"};
	
	char *commands[16];
	char *cmd1[16];
	int len1 = 0;
	char *cmd2[16];
	int len2 = 0;
	int current_cmd = 1;
	char *specChar = " ";

	char *tok;
	int count = 0;
	tok = strtok(line, delims);
	while(tok != NULL){	
		commands[count] = malloc(strlen(tok)*sizeof(char));
		strcpy(commands[count], tok);
		
		tok = strtok(NULL, delims);

		count++;
	}

	for(int i = 0; i < count; i++){
		for(int j = 0; j < 5; j++){
			if(!strcmp(special[j],commands[i])){
				current_cmd = 2;
				specChar = calloc(strlen(special[j]), sizeof(char));

				strcpy(specChar, special[j]);

				i++;
				break;
			}
		}
		if(current_cmd == 1){
			cmd1[len1] = malloc(strlen(commands[i])*sizeof(char));
			strcpy(cmd1[len1], commands[i]);

			len1++;
		}
		else if(i < count){
			cmd2[len2] = malloc(strlen(commands[i])*sizeof(char));
			strcpy(cmd2[len2], commands[i]);

			len2++;
		}
	}

	if(!strcmp(specChar,"<") && current_cmd == 2 && len1 > 0 && len2 > 0){
		//below code adapted from Week 7 Lab slides 
		int stdInSave = dup(0);
		int new_fd = open(cmd2[0], O_RDONLY);
		dup2(new_fd, 0);	
		close(new_fd);

		if(!builtIns(cmd1, len1, envp, abs)){
			runExternal(cmd1, len1);		
		}
		
		fflush(stdin);
		dup2(stdInSave, 0);
		close(stdInSave);

	}
	else if(!strcmp(specChar,">") &&  current_cmd == 2 && len1 > 0 && len2 > 0){
		//below code adapted from Week 7 Lab slides 
		int stdOutSave = dup(1);
		int new_fd = open(cmd2[0], O_WRONLY|O_CREAT|O_TRUNC,0777);
		dup2(new_fd, 1);	
		close(new_fd);
		
		if(!builtIns(cmd1, len1, envp, abs)){
			runExternal(cmd1, len1);	
		}

		fflush(stdout);
		dup2(stdOutSave, 1);
		close(stdOutSave);
	}
	else if(!strcmp(specChar,">>") && current_cmd == 2 && len1 > 0 && len2 > 0){
		//below code adapted from Week 7 Lab slides 
		int stdOutSave = dup(1);
		int new_fd = open(cmd2[0], O_WRONLY|O_CREAT|O_APPEND,0777);
		dup2(new_fd, 1);	
		close(new_fd);
		
		if(!builtIns(cmd1, len1, envp, abs)){
			runExternal(cmd1, len1);	
		}

		fflush(stdout);
		dup2(stdOutSave, 1);
		close(stdOutSave);

	}
	else if(!strcmp(specChar,"|") && current_cmd == 2 &&  len1 > 0 && len2 > 0){
		int pip[2];
   		int pid;
 		
  	 	if(pipe(pip) < 0){
			perror("(1) an error has occurred");
			exit(1);
		}

        	pid = fork();
    
        	if(pid == 0){
       			close(1); 
        
      			dup2(pip[1], 1);
        		close(pip[0]);
			cmd1[len1] = NULL; 
        		if(execvp(cmd1[0], cmd1) < 0){
				perror("(2) an error has occurred");
				exit(1);
			}
        	} 
        	else { 
     			pid = fork(); 
     			if(pid == 0){ 
       		 		close(0); 
 		      		dup2(pip[0], 0);
 	           		close(pip[1]); 
   				
				cmd2[len2] = NULL; 
            			if(execvp(cmd2[0], cmd2) < 0){
     					perror("(3) an error has occurred");
        				exit(1);
     				}
        	    	} 
 			else {
                		wait(&pid); 
               		}
                }
    
  		close(pip[0]);
   		close(pip[1]);
     
		int wstatus;
 		waitpid(0,&wstatus,WUNTRACED);
 		waitpid(0,&wstatus,WUNTRACED);	
	}
	else if(!strcmp(specChar,"&") && current_cmd == 2 && len1 > 0){
		int pid = fork();
		if(pid == 0){
			cmd1[len1] = NULL;
			execvp(cmd1[0], cmd1);
			perror("(4) an error has occurred");
			exit(1);
		}
	}
	else if(current_cmd != 2){
		if(!builtIns(cmd1, len1, envp, abs)){
			runExternal(cmd1, len1);	
		}
	}
	else{
		perror("an error has occurred");
		exit(1);
	}

	return 0;
}

int runExternal(char **cmd, int len){
	int pid = fork();
	if(pid == -1){
		perror("an error has occurred");
		exit(1);
	}
			
	if(pid == 0){
		cmd[len] = NULL;
		execvp(cmd[0], cmd);
		perror("an error has occurred");
		exit(1);
	}
	else{
		waitpid(pid, NULL, 0);
	}

	return 0;
}

