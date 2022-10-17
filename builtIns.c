#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "shell_header.h"
int builtIns(char **cmd, int length, char** envp){
	if(!strcmp(cmd[0], "cd")){
		char cwd[128];
		if(length == 2){
			chdir(cmd[1]);	
		}
		else if(length == 1){
			printf("%s\n", getcwd(cwd, 128));
		}
		return 1;
	}
	else if(!strcmp(cmd[0], "clr")){
		printf("\033[H\033[2J");
		return 1;
	}
	else if(!strcmp(cmd[0], "dir")){
		DIR *d;
		struct dirent *dir;
		if(length == 1){
			if((d = opendir(".")) == NULL){
				perror("no such directory\n");
				exit(1);
			}	
		}
		else if(length == 2){
			if((d = opendir(cmd[1])) == NULL){
				perror("no such directory\n");
				exit(1);
			}
		}
		
		
		while((dir = readdir(d)) != NULL){
			printf("%s\n", dir->d_name);
		}

		closedir(d);
		return 1;
	}
	else if(!strcmp(cmd[0], "environ")){
		int i = 0;
		while(envp[i] != NULL){
			printf("%s\n", envp[i]);
			i++;
		}
		return 1;
	}
	else if(!strcmp(cmd[0], "echo")){
		for(int i = 1; i < length; i++){
			printf("%s ", cmd[i]);
		}
		printf("\n");
		return 1;
	}
	else if(!strcmp(cmd[0], "help")){
		
		return 1;	
	}
	else if(!strcmp(cmd[0], "pause")){
 		char input;
		input = fgetc(stdin);
		while(input !='\n'){
			input = fgetc(stdin);
		}
		return 1;
	}	
	else if(!strcmp(cmd[0], "quit") | !strcmp(cmd[0], "exit")){
		exit(1);
	}

	return 0;
}
