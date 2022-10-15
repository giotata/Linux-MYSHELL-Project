#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_header.h"

int parse(){
	char *line = NULL;
	size_t size = 0;
	char delims[4] = " \t\n";
	char *special[5] = {">", "<", ">>", "|", "&"};

	printf("MyShell>");
	getline(&line, &size, stdin);
			
	if(!strcmp(line, "quit\n") || !strcmp(line, "exit\n")) {
		exit(0);
	}

	char *commands[16];
	char *cmd1[16];
	int len1 = 0;
	char *cmd2[16];
	int len2 = 0;
	int current_cmd = 1;	

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
				i++;
			}
		}
		if(current_cmd == 1){
			cmd1[len1] = malloc(strlen(commands[i])*sizeof(char));
			strcpy(cmd1[len1], commands[i]);
		
			printf("cmd1 = %s\n", cmd1[len1]);

			len1++;
		}
		else{
			cmd2[len2] = malloc(strlen(commands[i])*sizeof(char));
			strcpy(cmd2[len2], commands[i]);
		
			printf("cmd 2 = %s\n", cmd2[len2]);

			len2++;
		}
	}

	if(builtIns(cmd1, len1)){
		printf("\nBUILT IN\n");
	}

	return 0;
}

