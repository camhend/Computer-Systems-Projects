/* CS 347 -- Mini Shell!
 * Original author Phil Nelson 2000
 * Implementations authored by Cameron Henderson 2024
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
 * This program is a unix command-line shell. 
 * This main function is the primary read-eval-print loop.
 * The command line is read a single string then broken into 
 * individual arguments. The arguments are then processed
 * as a command.
 *  
 * This program exits when exit is called as a processed
 * command. This program will also exit when there is an
 * error in retreiving the input command line, in which
 * case it will exit with EXIT_FAILURE.
 */
int main() {
	char* line = NULL;
	size_t size = 0;
	ssize_t len;
	while (1) {	
		printf("%% ");	
		//If getinput fails, quit 
		if ( (len = getinput(&line, &size)) == -1 ) {
			return EXIT_FAILURE;
		}
		processline(line);		
	}	
	return EXIT_FAILURE;
}

/* getinput
* line     A pointer to a char* that points at a buffer of size *size or NULL.
* size     The size of the buffer *line or 0 if *line is NULL.

* This function prompts the user for input, e.g., %myshell%.  If the input fits in the buffer
* pointed to by *line, the input is placed in *line.  However, if there is not
* enough room in *line, *line is freed and a new buffer of adequate space is
* allocated.  The number of bytes allocated is stored in *size. 
* 
* Returns the length of the string stored in *line. On failure, -1 is returned instead,
* and an error statement is printed. If the failure was due to receving EOF from the input,
* then no error statement is printed. 
*/
ssize_t getinput(char** line, size_t* size) {
	ssize_t len = 0;
	if (*line == NULL) *size = 0;
	len = getline(line, size, stdin);
	if (len == -1 && !feof(stdin)) perror("Failed to read input line");
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
	int status;
	int argCount;
	char** arguments = argparse(line, &argCount); 
	if (argCount > 0) {
		if (builtIn(arguments, argCount) == 0) {
			cpid = fork();
			if (cpid > 0) {
				cpid = wait(&status);
				if (cpid == -1) {
					perror("myshell: Failed during wait for child process");
				}
			} else if (cpid == 0) {
				if (execvp(arguments[0], arguments) == -1) {
					fprintf(stderr, "myshell: Child process failed to execute file \"%s\": %s\n", 
						arguments[0], strerror(errno));
					exit(EXIT_FAILURE);
				}
			} else { // cpid == -1
				perror("myshell: Failed to fork to child process");
			}
		} 
	}
	// arguments form parseline must be freed, 
	// then free the arguments pointer itself.	
	for (int i = 0; i < argCount; i++) {
		free(arguments[i]);	
	}
	free(arguments); 
  }

