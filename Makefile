CC = gcc
WFLAGS = -Wall -Werror -g
EXE = MyShell
#SCAN_BUILD_DIR = scan_build-out

$(EXE):myShell.c parse.c builtIns.c shell_header.h
	$(CC) $(WFLAGS) -o $(EXE) myShell.c parse.c builtIns.c shell_header.h

clean:
	rm -f *.o $(EXE)

