#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

// Append the string argument onto the src string. 
// If the src string contains a '.', then the string is appended
// before the '.'. The resulting string is put into the dest char*.
// This assumes it a valid file name containing a '.' only to
// indicate a file format such as ".txt".
//
// dest: destination of result string
// sec: string to be appended onto.
// string: string that is appended onto src string.
//
// Returns: pointer to terminating null character of 
// 	the result string in dest.
char* append_file_name(char* dest, char* src, char* string) {
	char* src_copy = src;
	while ( *src_copy != '.' && *src_copy != '\0') src_copy++;
	dest = stpncpy(dest, src, src_copy - src);
	dest = stpcpy(dest, string);
	dest = stpcpy(dest, src_copy);
	return dest;
}

// Do a substitution sipher on specified number of chars
// in the buffer by adding shift_val to each char.
//
// buffer: buffer containing the target string
// bytes: number of bytes to perform cipher on (max: buffer size)
// shift_val: number to add to each char in buffer.
//
// Return: N/A
void cipher(char* buffer, size_t bytes, int shift_val) {
	int num_chars = bytes / sizeof(char);
	for (int i = 0; i < num_chars; i++) {
		buffer[i] = buffer[i] + shift_val;
	}	
	return;
}

// This program encrypts or decrypts the contents of a file
// by implementing a simple subsitution cipher on the characters
// of the file.
//
// Usage: filesec [-e | -d] [filename.txt]
//
// Input: 
// 	Argument 1: Options
// 		-e : encrpyt the file
// 		-d : decrypt the file
// 	Argument 2:  The file to be encrypted or decrypted.
//
// Output:
// 	A file "filename.txt" that was encrypted will have an 
// 	encrypted output file created as "filename_enc.txt". 
// 	If the file was decrypted, then a decrypted output file 
// 	will be created as "filename_dec.txt". 
//
// 	Program run information is printed after completion. 
int main(int argc, char** argv)
{
	// Start time of program
	struct timeval tv_start;
	gettimeofday(&tv_start, 0);

	int buffer_size = 100;

	// Get option from first argument.
	char usage_comment[] = "Usage: ./filesec -e|-d [filename.txt]\n";
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

	// Create output file name
	char output_file_name[128];    
	char* input_file_name = argv[2]; 
	append_file_name(output_file_name, input_file_name, file_name_suffix);

	// Begin file I/O operations
	int exit_code = 0;
	int fd_read = open(input_file_name, O_RDONLY);
	int fd_write;
	int read_calls = 0;
	int write_calls = 0;
	if (fd_read != -1) {
		fd_write = open(output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_write != -1) {
			char buffer[buffer_size];
			size_t bytes_read = read(fd_read, buffer, buffer_size * sizeof(char));
			read_calls++;
			size_t bytes_written;
			// Loop to read input file until EOF or until a read/write error occurs.
			while (bytes_read > 0 && bytes_written != -1) {
				// do subsitution cipher on chars in buffer
				cipher(buffer, bytes_read, shift_val); 

				bytes_written = write(fd_write, buffer, bytes_read);
				write_calls++;

				bytes_read = read(fd_read, buffer, buffer_size * sizeof(char));
				read_calls++;
			}
			if (bytes_read == -1) { 
				perror("Failed during read from file");
				exit_code = -1;
			} else if (bytes_written == -1) {
				perror("Failed during write to file");
				exit_code = -1;
			}
		} else {
			perror("Failed to create output file");
			exit_code = -1;
		}
	} else { 
		perror("Failed to open input file");
		exit_code = -1;
	}

	close(fd_read);
       	close(fd_write);

	// Get end time of program
	struct timeval tv_end;
	gettimeofday(&tv_end, 0);

	// Print program execution information if successful
	if (exit_code != -1) {
		long elapsed_usec = ((tv_end.tv_sec - tv_start.tv_sec) * 1000000) 
					+ (tv_end.tv_usec - tv_start.tv_usec);
		printf("%d,%ld,%d,%d\n", buffer_size, elapsed_usec, read_calls, write_calls);
	} else {
		printf("0,0,0,0\n");
	}

	return exit_code;
}
