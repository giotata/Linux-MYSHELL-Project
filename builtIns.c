#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "shell_header.h"
int builtIns(char **cmd, int length){
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
		return 1;
	}
	else if(!strcmp(cmd[0], "echo")){
		return 1;
	}
	else if(!strcmp(cmd[0], "help")){
		return 1;	
	}
	else if(!strcmp(cmd[0], "pause")){
		return 1;
	}	
	else if(!strcmp(cmd[0], "quit")){
		return 1;
	}

	return 0;
}
