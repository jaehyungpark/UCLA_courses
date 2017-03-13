#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

int verbose_flag = 0;

// in lab1a, implement options rdonly, wronly, command, and verbose
static struct option long_options[] = 
{
	{"rdonly",	required_argument,	0,				'r'},
	{"wronly",	required_argument,	0,				'w'},
	{"command",	no_argument,		0,				'c'},
	{"verbose",	no_argument,		&verbose_flag,	'v'}
	{0, 0, 0, 0}
};

