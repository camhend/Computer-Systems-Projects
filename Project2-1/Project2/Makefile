CFLAGS=-g -Wall

all: myshell

myshell: myshell.o argparse.o builtin.o
	gcc $(CFLAGS) -o myshell myshell.o builtin.o argparse.o

myshell.o: myshell.c argparse.h builtin.h
	gcc $(CFLAGS) -c myshell.c

argparse.o: argparse.c argparse.h
	gcc $(CFLAGS) -c argparse.c

builtin.o: builtin.c builtin.h
	gcc $(CFLAGS) -c builtin.c

clean:
	@echo -n Cleaning...
	@rm -f *.o myshell
	@echo done	
