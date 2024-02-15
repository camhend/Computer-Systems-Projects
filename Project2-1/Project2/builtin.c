#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <ctype.h>
#include "builtin.h"
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdint.h>
#include <time.h>

//Prototypes
static void exitProgram(char** args, int argcp);
static void cd(char** args, int argpcp);
static void pwd(char** args, int argcp);
static void ls(char** args, int argcp);
static void cp(char** args, int arrgcp);
static void env (char** args, int arrgcp);
static int numdigits(int num);
static int getstat(char* filename, struct stat* dirent_stat);
static void printperms(struct stat* dirent_stat);
static int cmp_str(const void *l, const void *r);

/* builtIn
 ** built in checks each built in command the given command, if the given command
 * matches one of the built in commands, that command is called and builtin returns 1.
 *If none of the built in commands match the wanted command, builtin returns 0;
  */
int builtIn(char** args, int argcp)
{
	if (argcp > 0) {	
		     if (strcmp(args[0], "exit ") == 0) {}
		else if (strcmp(args[0], "cd") == 0)    {}
		else if (strcmp(args[0], "pwd") == 0)   {}
		else if (strcmp(args[0], "ls") == 0)    { ls(args, argcp); }
		else if (strcmp(args[0], "cp") == 0)    {}
		else if (strcmp(args[0], "env") == 0) 	{}
		else { return 0; }
	} else {
		return 0;
	}
	return 1;
}

static void exitProgram(char** args, int argcp)
{
	
}

static void pwd(char** args, int argpc)
{
	
}


static void cd(char** args, int argcp)
{
 //write your code
}

static void ls(char** args, int argcp) 
{
	if (argcp > 2 ) {
		printf("ls: too many arguments\n");
		// TODO: return -1 on fail?
		return;
	}

	int lflag = 0;	
	if (argcp == 2) {
		if (strcmp(args[1], "-l") == 0) {
			lflag = 1;
		} else {
			printf("ls: argument \"%s\" not recognized\n", args[1]);
			// TODO: return -1 on fail?
			return;
		}
	}
	
	DIR* dir = opendir("./");
	struct dirent* dirent = readdir(dir);
	long start_offset;
	// skip "." and ".."
	while (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0) {
			// DIR offset of first dirent after "." and ".."
			start_offset = telldir(dir);
			dirent = readdir(dir);
	}	
	int total_entries = 0;
	while (dirent != NULL) {
		total_entries++;
	      	dirent = readdir(dir);
	}	

	// reset to first dirent after "." and ".."
	seekdir(dir, start_offset);
	dirent = readdir(dir);

	// make alphabetically sorted list of filenames
	// TODO: free filenames and filename strings
	char** filenames = malloc(total_entries * sizeof(char*));
	for (int i = 0; i < total_entries; i++) {
		filenames[i] = malloc( strlen(dirent->d_name) + 1 );
		strcpy(filenames[i], dirent->d_name);
		dirent = readdir(dir);
	}
	qsort(filenames, total_entries, sizeof filenames[0], cmp_str);

	// -l option was not entered: print regular form
	if (!lflag) 
	{
		for (int i = 0; i < total_entries; i++) {
			printf("%s ", filenames[i]);
		}
		printf("\n");
	}
        // -l flag was entered: print long form
	else 
	{
		int links_maxdigits = 0;
		int ownername_maxlen = 0;
		int grpname_maxlen = 0;
		int filesize_maxdigits = 0;
		struct stat* dirent_stat = malloc(sizeof(struct stat));

		// Get num entries
		// Also get max length of variable length attributes for column alignment
		// TODO: move to separate function?
		for (int i = 0; i < total_entries; i++) {
			if ( getstat(filenames[i], dirent_stat) == -1) {
				fprintf(stderr, "Failed to get file info from \"%s\": %s\n", 
					filenames[i], strerror(errno));
				// return -1 on fail? Prototype returns void?
				return;
			}	
			struct passwd* user = getpwuid(dirent_stat->st_uid);
			struct group* grp = getgrgid(dirent_stat->st_gid);

			int a = links_maxdigits;
			int b = numdigits( (int) dirent_stat->st_nlink);
			links_maxdigits = a > b ? a : b;

			a = ownername_maxlen;
			b = strlen(user->pw_name);
			ownername_maxlen = a > b ? a : b;

			a = grpname_maxlen;
			b = strlen(grp->gr_name);
			grpname_maxlen = a > b ? a : b;

			a = filesize_maxdigits;
			b = numdigits( (int) dirent_stat->st_size);
			filesize_maxdigits = a > b ? a : b;
		}
		
		// print files and attributes - 
		// 	TODO: make seperate function?
		// 	TODO: print total disk space at top
		for (int i = 0; i < total_entries; i++) {
			if ( getstat(filenames[i], dirent_stat) == -1) {
				fprintf(stderr, "Failed to get file info from \"%s\": %s\n", 
					filenames[i], strerror(errno));
				// return -1 on fail? Prototype returns void?
				return;
			}
			struct passwd* owner = getpwuid(dirent_stat->st_uid);
			struct group* grp = getgrgid(dirent_stat->st_gid);
			
			// get modification time as readable string- MONTH DATE HR:MIN
			struct timespec mod_time = dirent_stat->st_mtim;
			struct tm* mtim_tm =  localtime(&(mod_time.tv_sec));
			char mtim_str[30];
			strftime(mtim_str, sizeof(mtim_str), "%b %d %H:%M", mtim_tm);

			// begin print all attributes
			printperms(dirent_stat);						// permissions
			printf("%*d ", 
				links_maxdigits, (int) dirent_stat->st_nlink);  		// number of hard links
			printf("%*s ", ownername_maxlen, owner->pw_name);			// owner name
			printf("%*s ", grpname_maxlen, grp->gr_name);				// group name
			printf("%*d ", filesize_maxdigits, (int) dirent_stat->st_size); 	// file size
			printf("%s ", mtim_str);						// modification time
			printf("%s\n", filenames[i]);						// file name
		}
		free(dirent_stat);
		
	}

	if (closedir(dir) < 0) 
		perror("ls: failed to close directory");
	// TODO: return 1 on success?
	return;
}

static int numdigits(int num) {
	int digits = 1;
	while ((num = num / 10) > 0) digits++;
     	return digits;	
}

// get stat file of dirent in current working directory
static int getstat(char* filename, struct stat* dirent_stat) {
	char pathname[PATH_MAX];
	getcwd(pathname, PATH_MAX);
	strcat(pathname, "/");
	strcat(pathname, filename);
	return stat(pathname, dirent_stat);
}

static void printperms(struct stat* dirent_stat) {
	if (!dirent_stat) return;
	mode_t m = dirent_stat->st_mode;
	printf( S_ISDIR(m) ? "d" : "-" );	
	printf( m & S_IRUSR ? "r" : "-" );
	printf( m & S_IWUSR ? "w" : "-" );
	printf( m & S_IXUSR ? "x" : "-" );
	printf( m & S_IRGRP ? "r" : "-" );
	printf( m & S_IWGRP ? "r" : "-" );
	printf( m & S_IXGRP ? "x" : "-" );
	printf( m & S_IROTH ? "r" : "-" );
	printf( m & S_IWOTH ? "w" : "-" );
	printf( m & S_IXOTH ? "x" : "-" );
	printf(" ");
	return;
}

int cmp_str(const void* l, const void* r) {
	//const char* ls = (const char*) l;
	//const char* rs = (const char*) r;
	return strcasecmp(* (const char**) l, *(const char**) r);
}

static void cp (char** args, int argcp) {

	if (argcp > 2 || argcp < 2) {
		printf("Usage: cp [src_file_name] [dest_file_name]");
	}
	
	struct stat* filestat;
	if ( getstat(args[0], filestat) == -1) {
		fprintf(stderr, "Failed to get file info from \"%s\": %s\n", 
			args[0], strerror(errno));
			// return -1 on fail? Prototype returns void?
			return;
	}
	if ( getstat(args[1], filestat) != -1) {
		fprintf(stderr, "Destination file name already exists: \"%s\"\n", args[1]);
			// return -1 on fail? Prototype returns void?
			return;
	}

	char* src = args[0];
	char* dest = args[1];

        // Begin file I/O operations
        int exit_code = 0;
        int fd_read = open(src, O_RDONLY);
        int fd_write;
        if (fd_read != -1) {
				// TODO: change file permissions? Same as source file?
                fd_write = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd_write != -1) {
                        char buffer[BUFSIZ];
                        size_t bytes_read = read(fd_read, buffer, BUFSIZ * sizeof(char));
                        size_t bytes_written;
                        // Loop to read input file until EOF or until a read/write error occurs.
                        while (bytes_read > 0 && bytes_written != -1) {
                                bytes_written = write(fd_write, buffer, bytes_read);
                                bytes_read = read(fd_read, buffer, BUFSIZ * sizeof(char));
				
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

        return exit_code;
}

