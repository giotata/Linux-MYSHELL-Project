#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "shell_header.h"

int parse(char **envp){
	char *line = NULL;
	size_t size = 0;
	char delims[4] = " \t\n";
	char *special[5] = {">", "<", ">>", "|", "&"};

	printf("MyShell>");
	getline(&line, &size, stdin);
			
	char *commands[16];
	char *cmd1[16];
	int len1 = 0;
	char *cmd2[16];
	int len2 = 0;
	int current_cmd = 1;
	char *specChar;	

	char *tok;
	int count = 0;
	tok = strtok(line, delims);
	
	while(tok != NULL){	
		commands[count] = tok;
		tok = strtok(NULL, delims);
		count++;
	}
	
	for(int i = 0; i < count; i++){
		for(int j = 0; j < 5; j++){
			if(!strcmp(special[j],commands[i])){
				current_cmd = 2;
				specChar = malloc(strlen(special[j])*sizeof(char));
				strcpy(specChar, special[j]);
				i++;
			}
		}
		if(current_cmd == 1){
			cmd1[len1] = malloc(strlen(commands[i])*sizeof(char));
			strcpy(cmd1[len1], commands[i]);

			len1++;
		}
		else{
			cmd2[len2] = malloc(strlen(commands[i])*sizeof(char));
			strcpy(cmd2[len2], commands[i]);

			len2++;
		}
	}

	if(!strcmp(specChar,"<") && current_cmd == 2){
		printf("redirect stdin to %s\n", cmd2[0]);
	}
	else if(!strcmp(specChar,">") &&  current_cmd == 2){
		printf("redirect stdout to %s\n", cmd2[0]);
		//below code taken directly from 
		int stdOutSave = dup(1);
		int new_fd = open(cmd2[0], O_WRONLY);
		dup2(new_fd, 1);	
		close(new_fd);
		
		builtIns(cmd1, len1, envp);
		
		fflush(stdout);
		dup2(stdOutSave, 1);
		close(stdOutSave);
	}
//	else if(!strcmp(specChar,">>")){
		
//	}
	if(current_cmd != 2){
		builtIns(cmd1, len1, envp);
	}

	return 0;
}

