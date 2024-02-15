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

/*
* argCount is a helper function that takes in a String and returns the number of "words" in the string assuming that whitespace is the only possible delimeter.
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
*
* Argparse takes in a String and returns an array of strings from the input.
* The arguments in the String are broken up by whitespaces in the string.
* The count of how many arguments there are is saved in the argcp pointer
*/
char** argparse(char* line, int* argcp)
{
	*argcp = argCount(line);
	char** args = malloc(*argcp * sizeof(char*));
	int i = 0;
	while (*line && *line != '\n') {
		// skip leading whitespace
		while (*line && *line == ' ') 
			line++;
		if (*line) {
			// use pointer l to find end of word
			char* lp = line;
			while (*lp && *lp != ' ' && *lp != '\n') lp++;
			// malloc space 
			args[i] = malloc(lp - line + 1);
			int j = 0;
			while (line != lp) {
				args[i][j] = *line;
				line++;
				j++;
			}
			args[i][j] = '\0';
		}
		while (*line && *line != ' ') line++;
		i++;
	}	
	return args;
}

