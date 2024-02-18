/* CS 347 -- Mini Shell!
 * Original author Phil Nelson 2000
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include "argparse.h"
#include "builtin.h"


/* PROTOTYPES */

void processline (char *line);
ssize_t getinput(char** line, size_t* size);

/* main
 * This function is the main entry point to the program.  This is essentially
 * the primary read-eval-print loop of the command interpreter.
 */

int main () {

	char* line = NULL;
	size_t size = 0;
	ssize_t len;

		
	while (1) {	
		printf("%% ");	
		//If getinput fails, quit 
		//TODO: try to recover stdin after failure?
		if ( (len = getinput(&line, &size) == -1) ) {
			free(line);
			return EXIT_FAILURE;
		}
		processline(line);
			
		if (strcmp(line, "exit\n") == 0) break;
	}
	
	/*
	printf("Running ls -l: \n");
	char* args[2] = {"ls", "-l"};
	int argc = 2;
	ls(args, argc);

	printf("\nRunning ls: \n");
	char* args2[1] = {"ls"};
	int argc2 = 1;
	ls(args2, argc2);
	

	printf("\nRunning cp test some3.txt: \n");
	char* args2[3] = {"cp", "test", "some3.txt"};
	int argc2 = 3;
	cp(args2, argc2);
	*/


	free(line);

//write your code
//use getinput and processline as appropriate

	return EXIT_SUCCESS;
}


/* getinput
* line     A pointer to a char* that points at a buffer of size *size or NULL.
* size     The size of the buffer *line or 0 if *line is NULL.
* returns  The length of the string stored in *line.
*
* This function prompts the user for input, e.g., %myshell%.  If the input fits in the buffer
* pointed to by *line, the input is placed in *line.  However, if there is not
* enough room in *line, *line is freed and a new buffer of adequate space is
* allocated.  The number of bytes allocated is stored in *size. 
* Hint: There is a standard i/o function that can make getinput easier than it sounds.
*/
ssize_t getinput(char** line, size_t* size) {
	ssize_t len = 0;
	if (*line == NULL) *size = 0;
	len = getline(line, size, stdin);	
	if (len == -1 ) perror("Failed to read input line");
	return len;
}


/* processline
 * The parameter line is interpreted as a command name.  This function creates a
 * new process that executes that command.
 * Note the three cases of the switch: fork failed, fork succeeded and this is
 * the child, fork succeeded and this is the parent (see fork(2)).
 * processline only forks when the line is not empty, and the line is not trying to run a built in command
 */
void processline (char *line)
{
 /*check whether line is empty*/
  //write your code
    
	pid_t cpid;
	int   status;
	int argCount;

	char** arguments = argparse(line, &argCount); 

	if (argCount > 0) {
		if (builtIn(arguments, argCount)) {
			// execute builtin command
			
		} else {
			printf("Not a builtin function\n");
			// fork to execute command
			// execlp?
		}
	}
	
	for (int i = 0; i < argCount; i++) {
		free(arguments[i]);
		
	}
	free(arguments);
  
  
  }

