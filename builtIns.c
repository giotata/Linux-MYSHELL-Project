//Built in functions for MyShell
//Giorgio Tatarelli

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "shell_header.h"
int builtIns(char **cmd, int length, char** envp, char abs[]){
	if(!strcmp(cmd[0], "cd")){
		char cwd[128];
		if(length == 2){//case for when cd is passed with an arg
			if(chdir(cmd[1]) < 0){//attempt to change dir to dir name given
				perror("an error has occurred");//throw error on fail
				exit(1);	
			}	
		}
		else if(length == 1){//case for when cd is passed without an arg 
			printf("%s\n", getcwd(cwd, 128));//prints current working directory
		}
		return 1;
	}
	else if(!strcmp(cmd[0], "clr")){
		printf("\033[H\033[2J");//printing this clears the screen
		return 1;
	}
	else if(!strcmp(cmd[0], "dir")){
		DIR *d;
		struct dirent *dir;
		if(length == 1){//case for dir with no arg
			if((d = opendir(".")) == NULL){//opens current dir
				perror("no such directory\n");
				exit(1);
			}	
		}
		else if(length == 2){//case for dir with given dir name
			if((d = opendir(cmd[1])) == NULL){//opens given dir
				perror("no such directory\n");
				exit(1);
			}
		}
		
		
		while((dir = readdir(d)) != NULL){//iterates through files in dir
			printf("%s\n", dir->d_name);
		}

		closedir(d);
		return 1;
	}
	else if(!strcmp(cmd[0], "environ")){
	//this function is the reason I pass envp into builtIns and parse from main
		int i = 0;
		while(envp[i] != NULL){//loops over each field in envp and prints it
			printf("%s\n", envp[i]);
			i++;
		}
		return 1;
	}
	else if(!strcmp(cmd[0], "echo")){
		for(int i = 1; i < length; i++){//prints everything in user line
			printf("%s ", cmd[i]);//separates by one space regardless
		}
		printf("\n");
		return 1;
	}
	else if(!strcmp(cmd[0], "help")){
	//this function is the reason I pass abs (the absolute file path) into my functions
		FILE *fp;
		char *path;
		path = malloc(128*sizeof(char));
		snprintf(path, 128, "%s/readme_doc", abs);//creates path to readme_doc
		
		fp = fopen(path, "r");//opens readme
		char buffer[128];
		while(fgets(buffer, 128, fp) != NULL){//loops and prints each line of it
			printf("%s", buffer);
		}
		fclose(fp);		
		return 1;	
	}
	else if(!strcmp(cmd[0], "pause")){
 		char input;
		input = fgetc(stdin);
		while(input !='\n'){//continuosly checks if input char is enter, then returns
			input = fgetc(stdin);
		}
		return 1;
	}	
	else if(!strcmp(cmd[0], "quit") | !strcmp(cmd[0], "exit")){
		exit(0);//exits 
	}

	return 0;
}
