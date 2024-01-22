#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

// assume file name will be .txt? Allowed to use stat() to check if its a directory?
char* append_file_name(char* dest, char* src, char* string) {
	char* src_copy = src;
	while ( *src_copy != '.' && *src_copy != '\0') src_copy++;
	dest = stpncpy(dest, src, src_copy - src);
	dest = stpcpy(dest, string);
	dest = stpcpy(dest, src_copy);
	return dest;
}

int cipher(char* buffer, size_t bytes, int shift_val) {
	int num_chars = bytes / sizeof(char);
	for (int i = 0; i < num_chars; i++) {
		buffer[i] = buffer[i] + shift_val;
	}	
	return 0;
}

int main(int argc, char** argv)
{
	
	char output_file_name[128];    //You may assume that the length of the output file name will not exceed 128 characters.

	// TODO: remove hard coded argv[1]? - to allow for additional arguments?
	// early return ok?
	char usage_comment[] = "Usage: /filesec -e|-d [filename.txt]\n";
	char file_name_suffix[5];
	int shift_val;
	if (argc == 3) {	
		if (strcmp(argv[1],"-e") == 0) {
			strcpy(file_name_suffix, "_enc");
			shift_val = 100;
		} else if (strcmp(argv[1], "-d") == 0) {
			strcpy(file_name_suffix, "_dec");
			shift_val = -100;
		} else {
			printf(usage_comment);
			return -1;
		}	
	} else {	
		printf(usage_comment);
		return -1;
	}

	char* input_file_name = argv[2]; 
	append_file_name(output_file_name, input_file_name, file_name_suffix);
	printf("Output file name: %s\n", output_file_name);

	int exit_code = 0;
	int fd_read = open(input_file_name, O_RDONLY);
	int fd_write;
	if (fd_read != -1) {
		fd_write = open(output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_write != -1) {
			int buffer_size = 10;
			char buffer[buffer_size];
			size_t bytes_read = read(fd_read, buffer, buffer_size * sizeof(char));
			size_t bytes_written;
			while (bytes_read > 0 && bytes_written != -1) {
				cipher(buffer, bytes_read, shift_val);
				bytes_written = write(fd_write, buffer, bytes_read);
				bytes_read = read(fd_read, buffer, buffer_size * sizeof(char));
			}
			if (bytes_read == -1) { // Failed during read file
				perror("Failed during read from file");
				exit_code = -1;
			} else if (bytes_written == -1) { // Failed during write to file
				perror("Failed during write to file");
				exit_code = -1;
			}
		} else { // open file for write fail
			perror("Failed to create output file");
			exit_code = -1;
		}
	} else { // open file for read fail
		perror("Failed to open input file");
		exit_code = -1;
	}
	
	close(fd_read);
       	close(fd_write);	
	return exit_code;
}
