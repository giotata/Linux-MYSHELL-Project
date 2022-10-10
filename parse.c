#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_header.h"
int parse(){
	char *line = NULL;
	size_t size = 0;
	size_t read;

		
	printf("MyShell>");
	while((read = getline(&line, &size,stdin)) != -1){
		printf("%s", line);

		if(!strcmp(line, "quit\n") || !strcmp(line, "exit\n")) {
			exit(0);
		}

		printf("MyShell>");
	}

	return 0;
}
