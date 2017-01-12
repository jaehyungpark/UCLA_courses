#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

// open(2)
#include <fcntl.h>

// signal(2)
#include <signal.h>

// write(2), exit(2), read(2), close(2), dup(2)
#include <unistd.h>

// creat(2)
#include <sys/types.h>
#include <sys/stat.h>
#include <eerno.h>

// #define BUFF_SIZE 4096;



void stdin_to_stdout (int i_fd, int o_fd)
{
	char* buffer;
	buffer = (char*)malloc(sizeof(char));
	// copy stdin by read(2)-ing from fd(0) and write(2)-ing to fd(1)
	ssize_t input_file = read(i_fd, buffer, 1);
	// if no errors (other than EOF)
	while ((input_file = getchar()) != eof)
	{
		write(o_fd, buffer, 1);
		input_file = read(i_fd, buffer, 1);
	}
	free(buffer);
}

int 
main(int argc, char **argv)
{
	int c;
	int digit_optind = 0;

	// the use of getopt_long() was listed in the getopt(3) man page
	static struct option long_options[] = {
		{"input",		required_argument,	0,	'i' },
		{"output",		required_argument,	0,	'o' },
		{"segfault",	no_argument, 		0,	's' },
		{"catch", 		no_argument,		0,	'c' }
	};


	// from the file_descriptors link in the class website
	int i_fd = 0;
	int o_fd = 1;

	c = getopt_long(argc, argv, "i:o:sc", long_options, NULL);
	if (c == -1)
		break;

	// below switch statement with cases 'o' and 'i' uses the example
	// from the file_descriptors link in the class website
	// Also, some of the code reference is used from https://linux.die.net/man/3/getopt_long
	while (c != -1) {
		switch (c) {
			case 'i':
				i_fd = open(newfile, O_RDONLY);
				if (i_fd >= 0) {
					close(0);
					dup(i_fd);
					close(i_fd);
				} else {
					// if unable to open the specific file, report the failure using fprintf and perror
					fprintf();
					perror("Unable to open the specific file");
					exit(1);
				}
				break;
			case 'o':
				o_fd = creat(newfile, 0666);
				if (o_fd >= 0) {
					close(1);
					dup(o_fd);
					close(o_fd);
				} else {
					// if unable to create the specific output file, report the failure using fprintf and perror
					fprintf();
					perror("Unable to create the specific output file");
					exit(1);
				}
				break;
			case 's':

				break;
			case 'c':
				// use signal(2) to register a SIGSEGV handler that catches segfaults
				// and logs an error message (on stderr)
				signal(SIGSEGV, sigsegv_handler);
				break;
		}
	}
	stdin_to_stdout();
	exit(0);
}
