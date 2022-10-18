//main function for MyShell
//Giorgio Tatarelli

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_header.h"

int main(int argc, char**argv, char **envp){
	char abs[64];
	getcwd(abs, 64);

	while(1){
		parse(envp, abs);
	}

	return 0;
}
