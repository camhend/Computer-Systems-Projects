/* Cameron Henderson
 * February 2024
 * This file allows for parsing a single line of characters 
 * into individual words separated by a delimeter. 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "argparse.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define FALSE (0)
#define TRUE  (1)

/* line		string of arguments
 * 
 * argCount is a helper function that takes in a String and 
 * returns the number of "words" in the string assuming that 
 * whitespace is the only possible delimeter.
 */
static int argCount(char*line)
{
	int count = 0;
	while (*line && *line == ' ') line++;
	while (*line && *line != '\n') {
		if (*line) count++;
		while (*line && *line != ' ') line++;
		while (*line && *line == ' ') line++;
	}
	return count;
}

/*
* line		input line to be parsed.
* argcp 	pointer to int where the number of arguments will be stored.
* 
* Argparse takes in a String and returns a null terminated array of strings from the input.
* The arguments in the String are broken up by whitespaces in the string.
* The count of how many arguments there are is saved in the argcp pointer.
* Each member arguments[0...argcp + 1] must be freed by the caller, 
* as well as arguments pointer itself.
* 
* Returns null terminated array of string arguments.	
*/
char** argparse(char* line, int* argcp)
{
	*argcp = argCount(line);
	char** args = malloc((*argcp + 1) * sizeof(char*));
	int i = 0;
	while (*line && *line != '\n') {
		// skip whitespace
		while (*line && *line == ' ' && *line != '\n') 
			line++;
		if (*line && *line != '\n') {
			// use pointer l to find end of word
			char* lp = line;
			while (*lp && *lp != ' ' && *lp != '\n') lp++;
			// Size of word is address of end the of word 
			// minus address of start of word.
			// Add one for terminating null char.
			args[i] = malloc(lp - line + 1);
			int j = 0;
			// Copy entire word into arguments[i]
			while (line != lp) {
				args[i][j] = *line;
				line++;
				j++;
			}
			args[i][j] = '\0';
			i++;
		}
	}	
	// add terminating NULL pointer
	args[i] = NULL;
	return args;
}

