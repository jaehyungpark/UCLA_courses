#include <stdlib.h>
#include <stdio.h> 
#include <getopt.h> 
#include <unistd.h> 	
#include <fcntl.h> 	
#include <signal.h>
#include <ctype.h>
#include <string.h> 
#include <errno.h> 
#include <sys/wait.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#define TABLE_SIZE 1000

// flag variables
int verbose_flag = 0;
int profile_flag = 0;
int append_flag = 0; 
int cloexec_flag = 0; 
int creat_flag = 0; 
int directory_flag = 0; 
int dsync_flag = 0; 
int excl_flag = 0; 
int nofollow_flag = 0; 
int nonblock_flag = 0; 
int rsync_flag = 0; 
int sync_flag = 0; 
int trunc_flag = 0; 
int wait_flag = 0; 

int table_size = TABLE_SIZE; 

// index variables
int table_index = 0; 

void signalHandler(int signum) {
	fprintf(stderr, "signal handler caught signal %d \n", signum);
	exit(signum); 
}

// someone on piazza mentioned that passing the function will make it better
// it seems to be better to return all the flags to 1 and OR them for file-opening options
int flags(void) {
	return (append_flag | cloexec_flag | creat_flag | directory_flag | dsync_flag | excl_flag | nofollow_flag | nonblock_flag | rsync_flag | sync_flag | trunc_flag); 
}

// it seems to be better to reset all these flags to 0 at once
void reset_flags(void) {
	append_flag = 0; cloexec_flag = 0; creat_flag = 0; directory_flag = 0; dsync_flag = 0; 	excl_flag = 0; nofollow_flag = 0; nonblock_flag = 0; rsync_flag = 0; sync_flag = 0; trunc_flag = 0; 
}

typedef 
struct process_info {
	char* command_argv[1000];
	pid_t child_pid; 
} process_info;

// used double for more precision point
double timeval (struct timeval t){
	double seconds = (double) t.tv_sec; 
	double useconds = (double) t.tv_usec; 
	return (seconds + useconds*0.000001); 
}

// lab1b uses all flags except for profile
static struct option long_options[] = {
	// file-opening options 
	{"rdonly",		required_argument,		0,					'r'}, 
	{"wronly", 		required_argument, 		0, 					'w'}, 
	{"rdwr", 		required_argument, 		0, 					'z'}, 
	{"pipe", 		no_argument, 			0, 					'p'}, 

	// file-opening flags 
	{"append",		no_argument,			&append_flag,		O_APPEND},
	{"cloexec",		no_argument,			&cloexec_flag,		O_CLOEXEC},
	{"creat",		no_argument,			&creat_flag,		O_CREAT},
	{"directory",	no_argument,			&directory_flag,	O_DIRECTORY},
	{"dsync",		no_argument,			&dsync_flag,		O_DSYNC},
	{"excl",		no_argument,			&excl_flag,			O_EXCL},
	{"nofollow",	no_argument,			&nofollow_flag,		O_NOFOLLOW},
	{"nonblock",	no_argument,			&nonblock_flag,		O_NONBLOCK},
	{"rsync",		no_argument,			&rsync_flag,		O_RSYNC},
	{"sync",		no_argument,			&sync_flag,			O_SYNC},
	{"trunc",		no_argument,			&trunc_flag,		O_TRUNC},
	{"profile",		no_argument,			&profile_flag,		 1 }, 

	// sub-command options 
	{"command", 	required_argument, 		0, 					'c'}, 
	{"wait", 		no_argument, 			&wait_flag,			'1'},

	// miscellaneous options
	{"close",		required_argument,		0,					'e'}, 
	{"verbose",		no_argument,			&verbose_flag,		 1 },
	{"abort",		no_argument,			0,					'a'}, 
	{"catch",		required_argument,		0,					't'}, 
	{"ignore",		required_argument,		0,					'i'}, 
	{"default",		required_argument,		0,					'd'}, 
	{"pause",		no_argument,			0,					'u'},
	{0, 0, 0, 0}
};

int main(int argc, char** argv){

	// store file descriptor
	int* fd_table = (int*)malloc(table_size * sizeof(int));
	if (!fd_table) {
		fprintf(stderr, "failed to allocate memory space for file descriptor table"); 
		exit(1); 
	}
	int fd; 
	char* file; 

	char* command;					 // stores the command and it's args
	char* command_args[1000];
	int ch_index = 0;				 // child process index, start from 0
	int count;						 // track how many arguments in command option 
	process_info child_arr[1000];

	int opt; 						// stores value returned by getopt_long
	int long_index = 0; 			// stores index of current option in long_options array 
	struct option current_longopt;
	int d_pipe[1000]; 				// stores if fd has pipe at the end

	int i_fd, o_fd, e_fd; 			// stores each of the stdin, stdout, and stderr arguments
	int c_status = 0;				// check the status to exit if the program behaves wrong
	struct rusage usage;

	while(1) { 
		opterr, count = 0;
		opt = getopt_long(argc, argv, ":", long_options, &long_index);  

		// if the stored value by getopt_long is -1, when no arguments, break the loop
		if(opt == -1) {
			break; 
		}

        // added code to dynamically increase the buffer size in case it exeeeds it's limits
        if (table_index == TABLE_SIZE) {
            table_size = table_size*2;
            int* arr = (int*)realloc(fd_table, table_size * sizeof(int));
            if(!arr) {
                fprintf(stderr,"failed to reallocated fd table space.\n");
                exit(1);
            }
        }
		
		switch(opt) {
			// rdonly option
			case 'r': 
				file = optarg; 
				fd = open(file, O_RDONLY | flags(), 0666); 
				// when failed to open the file
				if(fd < 0) {
					perror(file);
					exit(c_status); 
					exit(1);
				}
				if(fd >= 0) {
					fd_table[table_index] = fd;
					d_pipe[table_index] = 0; 
					table_index++;
				}
				// reset flags to 0
				reset_flags(); 
				break; 

			// wronly option
			case 'w': 
				file = optarg; 
				fd = open(file, O_WRONLY | flags(), 0666); 
				if(fd < 0){
					// when failed to open the file
					perror(file);
					exit(c_status);
					exit(1);
				}
				if(fd >= 0){
					fd_table[table_index] = fd;
					d_pipe[table_index] = 0; 
					table_index++;
				}
				// reset flags to 0
				reset_flags(); 
				break; 

			// rdwr option
			case 'z':
				file = optarg; 
				fd = open(file, O_RDWR | flags(), 0666); 
				if(fd < 0) {
					// when failed to open the file
					perror(file);
					exit(c_status); 
					exit(1);
				}
				if(fd >= 0) {
					fd_table[table_index] = fd;
					d_pipe[table_index] = 0; 
					table_index++;
				}
				// reset flags to 0
				reset_flags(); 
				break; 

			// close option
			case 'e': {
				// convert char* to int
				int num = atoi(optarg); 
				if(num < 0) {
					fprintf(stderr, "invalid file descriptor"); 
					exit(1);
					break; 
				}
				if(num >= table_index) {
					fprintf(stderr, "file descriptor doesn't exist"); 
					exit(1);
					break; 
				}
				close(fd_table[num]); 
				break;
			}

			// pipe option
			case 'p': {
				// create a pipe
				int fd_pipe[2]; 
				if(pipe(fd_pipe) == -1) {
					perror("pipe error"); 
					exit(1); 
					break; 
				}
				fd_table[table_index] = fd_pipe[0]; // read end of fd
				d_pipe[table_index++] = 1; 
				fd_table[table_index] = fd_pipe[1]; // write end of fd
				d_pipe[table_index++] = 1; 
				
				break;
			}

			// abort option
			case 'a': 
				raise(SIGSEGV);  

			// pause option
			case 'u': 
				pause(); 
				break; 

			// ignore option
			case 'i': {
				int sig_num = atoi(optarg); 
				signal(sig_num, SIG_IGN); 
				break; 
			}

			// default option
			case 'd': {
				int sig_num = atoi(optarg); 
				signal(sig_num, SIG_DFL); 
				break;
			}   

			// catch option
			case 't': {
				int sig_num = atoi(optarg); 
				signal(sig_num, signalHandler);
				break; 
			}

			// command option
			case 'c': {
				if (optind < argc) {
					i_fd = atoi(argv[optind-1]);
					o_fd = atoi(argv[optind++]);
					e_fd = atoi(argv[optind++]);

                    // in case each of stored arguments for in, out, error exceeds the 
                    // fd table index, prompt error that can't specify the fd
					if (i_fd >= table_index) {
						fprintf(stderr, "invalid stdin file descriptor\n");
					}
					if (o_fd >= table_index) {
						fprintf(stderr, "invalid stdout file descriptor\n");
					}
					if (e_fd >= table_index) {
						fprintf(stderr, "invalid stderr file descriptor\n");
					}
					command = argv[optind++]; 
					command_args[count] = command;
					count++; 
				}
				
                // parse through the command line arguments
				int ground = 0; 
				while(1) {
					if(optind < argc) {
						int temp = optind;
						int length = strlen(argv[optind]); 
						opt = getopt_long(argc, argv, ":", long_options, &long_index);
                    	// check if opt is an alphabet, if not
						if(isalpha(opt)) { 
							ground = optind - temp;
							optind = temp; 
							break; 
						} else if (opt == 0) {
							// exit loop when long option sets a flag 
							ground = optind - temp;
							optind = temp; 
							break;
						} else {
							optind = temp; 
							command_args[count] = argv[optind++];  
							count++;
						}
					}	else 
						// when there is no more arguments to parse through
						break; 
				}

				// continue looking at the next part of the command
				int next_command;

				current_longopt = long_options[long_index]; 
				if (current_longopt.has_arg == no_argument) {
					next_command = optind + ground - 1;
				}
				if (current_longopt.has_arg == required_argument) {
					next_command = optind + ground - 2; 
				}
				for(; optind < next_command; optind++) {
					command_args[count++] = argv[optind];
				}
				
				// close command_args to NULL	
				command_args[count] = NULL;
				
				// if verbose flag is set
				if (verbose_flag) {
					fprintf(stdout, "--command %d %d %d ", i_fd, o_fd, e_fd); 
					int j;
					for(j = 0; j < count; j++) {
						fprintf(stdout, "%s ", command_args[j]); 
					}
					printf("\n"); 
				}	
				
				pid_t pid = fork(); 
				if(pid == 0) { 
					// in child process, create duplicates for in, out, and error_fd respecfully to
					// fd index 0, 1, and 2
					dup2(fd_table[i_fd], 0); 
					dup2(fd_table[o_fd], 1); 
					dup2(fd_table[e_fd], 2); 

					// closing all file descriptor table entries 
					int i; 
					for(i=0; i < table_index; i++) {
						// close file descriptor
						close(fd_table[i]); 
					}
					if(execvp(command, command_args)) { 
						perror("execvp: "); 
					}
				} else if(pid > 0) {
					// parent process
					// store child process' info to child_arr 
					if(d_pipe[i_fd])
						close(fd_table[i_fd]); 
					if(d_pipe[o_fd])
						close(fd_table[o_fd]); 
					
					// store info about child process into child_arr array 
					child_arr[ch_index].child_pid = pid; 
					int j; 
					for(j = 0; j < count; j++) {
						child_arr[ch_index].command_argv[j] = command_args[j];
					}
					ch_index++; 
				}
				break; 	
			}

			default: 
				break; 
        }
		// if profile flag is sets
		if(profile_flag) {
			int who = RUSAGE_SELF;
			int ret;
			ret = getrusage(who, &usage);
			if(!ret) {
				double user_time = timeval(usage.ru_utime); 
				double system_time = timeval(usage.ru_stime);
				printf("User time used: %f seconds. System time used: %f seconds\n", user_time, system_time); 
				} else {
				perror("rusage: ");
		    	}
		    }
	}
	
	// when verbose flag is hit
	if(verbose_flag) {
		current_longopt = long_options[long_index]; 
		// cases requiring arguments 
		// using strcmp to compare the command string
		int c_nil = strcmp(current_longopt.name, "command");
		// this will return a non zero value when it's not command
		if (current_longopt.has_arg == required_argument && c_nil) {
			printf("--%s ", current_longopt.name); 
			printf("%s\n", optarg); 
		}
		// cases for no taking arguments 
		// using strcmp to compare the verbose string
		int v_nil = strcmp(current_longopt.name, "verbose"); 
		// this will return a non zero value when it's not verbose 
		if (current_longopt.has_arg == no_argument && v_nil) {
			printf("--%s\n", current_longopt.name); 
		}
	}
	
	int i; 
	// free file descriptor table array
	for(i = 0; i < table_index; i++) {
		close(fd_table[i]); 
	}
	free(fd_table); 

	// if wait flag is set
	if(wait_flag) {
		int i, j, k; 
		for(i = 0; i < ch_index; i++) {
			int c_status; 
			int w_status = waitpid(-1, &c_status, 0);
			int exitStatus = WEXITSTATUS(c_status); 
			if (exitStatus > c_status) {
				c_status = exitStatus; 
			}
			if (w_status == -1) {
				perror("wait error: "); 
			}
			printf("%d ", exitStatus); 
			for(j = 0; j < ch_index; j++) {
				if(child_arr[j].child_pid == w_status) {
					k = 0;
					while(child_arr[j].command_argv[k] != NULL) {
						printf("%s ", child_arr[j].command_argv[k]);
						k++; 
					}
					break; 
				}
			}
		}
	}

	// if profile flag is set
	if(profile_flag) {
		int who = RUSAGE_CHILDREN;
		int ret;
		ret = getrusage(who, &usage);
		if(!ret) {
			double user_time = timeval(usage.ru_utime); 
			double system_time = timeval(usage.ru_stime);
			printf("User time used: %f seconds. System time used: %f seconds\n", user_time, system_time); 
			} else {
			perror("rusage: ");
		}
	}
	
	// when c_status kicks in, program ends 
	if(ch_index && c_status) {
		exit(c_status); 
	}
	return 0; 
}
