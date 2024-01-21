#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

// assume file name will be .txt?
char* append_file_name(char* dest, char* src, char* string) {
	char* src_copy = src;
	while ( *src_copy != '.' && *src_copy != '\0') src_copy++;
	dest = stpncpy(dest, src, src_copy - src);
	dest = stpcpy(dest, string);
	dest = stpcpy(dest, src_copy);
	return dest;
}

int cipher(char* buffer, int num_chars, int shift_val) {
	for (int i = 0; i < num_chars; i++) {
		buffer[i] = buffer[i] + shift_val;
	}	
	return 0;
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
	
	char* input_file_name = argv[2]; 
	append_file_name(output_file_name, input_file_name, file_name_suffix);
	printf("Output file name: %s\n", output_file_name);

	int fd_read = open(input_file_name, O_RDONLY);
	if (fd_read == -1) {
		perror("Failed to open file");
		return -1;
	}

	int fd_write = open(output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd_write == -1) {
		perror("Failed to create output file");
		return -1;
	}
	
	int buffer_length = 10;
	char buffer[buffer_length];
	size_t bytes_read = read(fd_read, buffer, sizeof(buffer));
	size_t bytes_written;
	while (bytes_read > 0) {
		cipher(buffer, bytes_read / sizeof(char), shift_val);
		bytes_written = write(fd_write, buffer, bytes_read);
		bytes_read = read(fd_read, buffer, sizeof(buffer));
	}
	if (bytes_read == -1) {
		perror("Failed to read file");
		return -1;
	}
	
	//  
	
	// 


   	close(fd_read);
       	close(fd_write);	
	return 0;

}
