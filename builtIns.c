#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_header.h"
int builtIns(char **cmd, int length){
	if(!strcmp(cmd[0], "cd")){
		return 1;
	}
	else if(!strcmp(cmd[0], "clr")){
		return 1;
	}
	else if(!strcmp(cmd[0], "dir")){
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
