#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// getopt(3)
#include <getopt.h>

// open(2)
#include <fcntl.h>

// signal(2)
#include <signal.h>

// write(2), exit(2), read(2), close(2), dup(2)
#include <unistd.h>

// creat(2)
#include <sys/types.h>
#include <sys/stat.h>

// perror(3)
#include <errno.h>

#define BUFF_SIZE 4096

void sig_handler (int signum)
{
	perror("segfault caught by signal handler");
	exit(3);
}

int
main(int argc, char **argv) 
{
	int c = 0;
	// in case of segfault flag occurs, segfault_flag will be assigned as 1 and trigger to cause segfault
	int segfault_flag = 0;

	// the use of getopt_long() was listed in the getopt(3) man page
	static struct option long_options[] = 
	{
		{"input",		required_argument,	0,	'i' },
		{"output",		required_argument,	0,	'o' },
		{"segfault",	no_argument, 		0,	's' },
		{"catch", 		no_argument,		0,	'c' }
	};

	// used the gnu.org reference manpage for parsing long options with option_index and getopt_long 
	// getopt_long stores the option index here (could use NULL in getopt_long instead of &option_index)
	// int option_index = 0;

	// from the file_descriptors link in the class website
	int i_fd = 0;
	int o_fd = 1;

	int option_index = 0;

	// below switch statement with cases 'o' and 'i' uses the example
	// from the file_descriptors link in the class website
	// Also, some of the code reference is used from https://linux.die.net/man/3/getopt_long
	while ((c = getopt_long(argc, argv, "i:o:sc", long_options, &option_index)) != -1)
	{
		switch (c) 
		{
			case 'i':
				i_fd = open(optarg, O_RDONLY);
				if (i_fd >= 0) 
				{
					close(0);
					dup(i_fd);
					close(i_fd);
				} else {
					// if unable to open the specific file, report the failure using fprintf and perror
					fprintf(stderr, "Unable to open the specific file, stderr");
					perror("Unable to open the specific file");
					exit(1);
				}
				break;

			case 'o':
				o_fd = creat(optarg, 0666);
				if (o_fd >= 0) 
				{
					close(1);
					dup(o_fd);
					close(o_fd);
				} else {
					// if unable to create the specific output file, report the failure using fprintf and perror
					fprintf(stderr, "Unable to create the specific output file, stderr");
					perror("Unable to create the specific output file");
					exit(1);
					}
				break;

			case 's':
				// change segment_flag from 0 to 1 to cause segfault
				segfault_flag = 1;				
				break;

			case 'c':
				// use signal(2) to register a SIGSEGV handler that catches segfaults
				// and logs an error message (on stderr, fd2) and exit with return code of 3.
				signal(SIGSEGV, sig_handler);
				break;

			default:
				printf("use appropriate flags and input/output files");
				break;
		}
	}

	if (segfault_flag)
	{
		// from the spec, set a char * pointer to NULL and then store through it	
		int *ptr = NULL;
		// I wasn't sure how to cause a segmentation fault, and I was looking on stackoverflow that it usually happens
		// when faulty accessing a memory that doesn't belong to you.
		*ptr = 1;
	}

	// the main part of the program, read from stdin and copy to stdout
	char buffer[BUFF_SIZE] = {};
	// copy stdin by read(2)-ing fd0, assuming the count 1 is smaller than ssize_max
	// but not 0 since it will return zero with no other effects
	ssize_t temp_size;
	// if no errors other than eof and the size of temp is bigger than 0
	while ((temp_size = read(0, buffer, BUFF_SIZE)) > 0)
	{
		// write to stdout by write(2)-ing fd1
		write(1, buffer, temp_size);
	}
	// exit program with return code of 0
	exit(0);
}
