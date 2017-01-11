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

#define BUFF_SIZE 4096;

int 
main(int argc, char **argv)
{
	int c;

	// the use of getopt_long() was listed in the getopt(3) man page
	static struct option long_options[] = {
		{"input",		required_argument,	0,	'i' },
		{"output",		required_argument,	0,	'o' },
		{"segfault",	no_argument, 		0,	's' },
		{"catch", 		no_argument,		0,	'c' }
	};


	// from the file_descriptors link in the class website, the example
	// uses input and output redirection variables as ifd and ofd
	int ifd = 0;
	int ofd = 1;

	c = getopt_long(argc, argv, "i:o:sc", long_options, NULL);
	if (c == -1)
		break;

	// below switch statement with cases 'o' and 'i' uses the example
	// from the file_descriptors link in the class website
	switch (c) {
		case 'i':
			ifd = open(newfile, O_RDONLY);
			if (ifd >= 0) {
				close(0);
				dup(ifd);
				close(ifd);
			}
			break;
		case 'o':
			ofd = creat(newfile, 0666);
			if (ofd >= 0) {
				close(1);
				dup(ofd);
				close(ofd);
			}
			break;
		case 'c':
			signal(SIGSEGV, sigsegv_handler);
			break;
	}

}
