CC = gcc
WFLAGS = -Wall -Werror
EXE = MyShell
#SCAN_BUILD_DIR = scan_build-out

$(EXE):myShell.o parse.o
	$(CC) $(WFLAGS) -o $(EXE) myShell.o parse.o

myShell.o:myShell.c shell_header.h
	$(CC) $(WFLAGS) -c myShell.c

parse.o:parse.c shell_header.h
	$(CC) $(WFLAGS) -c parse.c

clean:
	rm -f *.o $(EXE)

