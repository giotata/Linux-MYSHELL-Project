//main function for MyShell
//Giorgio Tatarelli

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_header.h"

int main(int argc, char**argv, char **envp){
	char abs[64];
	getcwd(abs, 64);
	
	if(argc > 1){
		FILE *fp;
		char buffer[64];
		
		fp = fopen(argv[1], "r");
		if(fp == NULL){
			perror("an error has occurred");
			exit(1);
		}
			
		while(fgets(buffer, 128, fp) != NULL){
			parse(buffer, envp, abs);
		}
		fclose(fp);
	}	
	else{	
		while(1){
			char *line;
			line = malloc(64*sizeof(char));
			size_t size = 0;
			char curr[64];
			getcwd(curr, 64);
	
			printf("MyShell %s>", curr);
			getline(&line, &size, stdin);
			parse(line, envp, abs);
		}
	}

	return 0;
}
