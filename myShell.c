//main function for MyShell
//Giorgio Tatarelli

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_header.h"

int main(int argc, char**argv, char **envp){
	char abs[64];
	getcwd(abs, 64);//stores absolute filepath of MyShell executable 
	
	char *path;
	path = malloc(128*sizeof(char));
	snprintf(path, 128, "%s/MyShell", abs);

	setenv("SHELL", path, 1);
	
	if(argc > 1){//when passes with an argument, MyShell enters batch mode
		FILE *fp;
		char buffer[64];
		
		fp = fopen(argv[1], "r");//attempts to open given batch file name
		if(fp == NULL){//throws error if unable
			perror("an error has occurred");
			exit(1);
		}
			
		while(fgets(buffer, 64, fp) != NULL){//reads each line of batch file
			parse(buffer, envp, abs);//parses and runs each line
		}
		fclose(fp);
	}	
	else{//without arguments, launch in interactive mode
		while(1){//loop until user quits or crash
			char *line;
			line = malloc(64*sizeof(char));
			size_t size = 0;
			char curr[64];
			getcwd(curr, 64);//stores current working dir
	
			printf("MyShell %s>", curr);//prints current dir in command line 
			getline(&line, &size, stdin);//gets user input command line
			parse(line, envp, abs);//parses and runs line
		}
	}

	return 0;
}
