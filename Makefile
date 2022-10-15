CC = gcc
WFLAGS = -Wall -Werror -g
EXE = MyShell
#SCAN_BUILD_DIR = scan_build-out

$(EXE):myShell.o parse.o builtIns.o
	$(CC) $(WFLAGS) -o $(EXE) myShell.o parse.o builtIns.o

myShell.o:myShell.c shell_header.h
	$(CC) $(WFLAGS) -c myShell.c

parse.o:parse.c shell_header.h
	$(CC) $(WFLAGS) -c parse.c

builtIns.o:builtIns.c shell_header.h
	$(CC) $(WFLAGS) -c builtIns.c

clean:
	rm -f *.o $(EXE)

