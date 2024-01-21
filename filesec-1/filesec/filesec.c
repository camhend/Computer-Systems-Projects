#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

char* append_file_name(char* dest, char* src, char* string) {
	char* src_copy = src;
	while ( *src_copy != '.' && *src_copy != '\0') src_copy++;
	dest = stpncpy(dest, src, src_copy - src);
	dest = stpcpy(dest, string);
	dest = stpcpy(dest, src_copy);
	return dest;
}

int main(int argc, char** argv)
{
	
	char output_file_name[128];    //You may assume that the length of the output file name will not exceed 128 characters.
	
	// Read command line arguments
		// ./filesec -e|-d [filename.txt]
		// TODO: remove hard coded argv[1]? - to allow for additional arguments?

	char usage_comment[] = "Usage: /filesec -e|-d [filename.txt]\n";
	if (argc != 3) {	
		printf(usage_comment);
		return -1;
	}

	char file_name_suffix[5];
	int shift_val;
	if (strcmp(argv[1],"-e") == 0) {
		strcpy(file_name_suffix, "_enc");
		shift_val = 100;
	} else if (strcmp(argv[1], "-d") == 0) {
		strcpy(file_name_suffix, "_dec");
		shift_val = -100;
	} else {
		printf(usage_comment);
		return -1; 
		// TODO: bad early return?
	}
	
	append_file_name(output_file_name, argv[2], file_name_suffix);
	printf("%s", output_file_name);


	// open appropriate file for read-only
	
	//  
	
	// 


    
	return 0;

}
