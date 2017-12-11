#include <stdio.h> 
#include <stdlib.h>
#include <getopt.h> 
#include <unistd.h> 	
#include <fcntl.h> 	
#include <signal.h>
#include <ctype.h>
#include <string.h> 

#define TABLE_SIZE 1000 

// flag variables
int verbose_flag = 0; 

int table_size = TABLE_SIZE; 

// index variables
int table_index = 0; 

typedef
struct process_info { 
	char* command_argv[100];
    pid_t child_pid;
} process_info;

static struct option long_options[] = {
	{"rdonly",		required_argument,		0,				'r'}, 
	{"wronly",		required_argument,		0,				'w'}, 
	{"command",		required_argument,		0,				'c'}, 
	{"verbose",		no_argument, 			&verbose_flag,	 1 },
	{0, 0, 0, 0}
};

int main(int argc, char** argv){

	int* fd_table = (int*)malloc(table_size*sizeof(int)); 	// store file descriptor
	if (!fd_table) {
		fprintf(stderr, "failed to allocated memory for file descriptor table"); 
		exit(1); 
	}
	int fd; 
	char* file;

	char* command;							// stores the cmd and its arg
	char* command_args[100];
    int ch_index = 0;                       // child process index, start from 0
	int count = 0;	       					// track how many arguments in command option 
	process_info child_arr[100]; 
	
	int opt;								// store the value returned from getopt_long
	int long_index = 0;						// stores index of current option in long_options array
	struct option current_longopt;

    int i_fd, o_fd, e_fd;                   // each stores the arguments for in, out, error_fd

	while(1) { 
		opt = getopt_long(argc, argv, ":", long_options, &long_index);  
        // when no arguments, break
        if (opt == -1)
			break; 

		switch(opt) {
            // rdonly option
			case 'r': 
				file = optarg;
				fd = open(file, O_RDONLY, 0644); 
                // in case it fails to open file
				if (fd < 0) {
					perror(file);
					exit(0); 
				}
				if (fd >= 0) {
					fd_table[table_index] = fd;
					table_index++;
				}
				break; 
            // wronly option
			case 'w': 
				file = optarg;
				fd = open(file, O_WRONLY, 0644); 
                // in case it fails to open file
				if (fd < 0) {
					perror(file);
					exit(0);
				}
				if (fd >= 0) {
					fd_table[table_index] = fd;
					table_index++;
				}
				break; 
            // command option
			case 'c': { // check for in, out, error fd
				if (optind < argc) {
					i_fd = atoi(argv[optind-1]);
					o_fd = atoi(argv[optind++]);
					e_fd = atoi(argv[optind++]);

                    // in case each of stored arguments for in, out, error exceeds the 
                    // fd table index, prompt error that can't specify the fd
					if (i_fd >= table_index) {
						fprintf(stderr, "error while specifiying input file descriptor\n");
					}
					if (o_fd >= table_index) {
						fprintf(stderr, "error while specifiying output file descriptor\n");
					}
					if (e_fd >= table_index) {
						fprintf(stderr, "error while specifiying error file descriptor\n");
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
						opt = getopt_long(argc, argv, ":", long_options, &long_index);
                        // check if opt is an alphabet, if not
						if(isalpha(opt)) { 
							ground = optind - temp; 
							optind = temp; 
							break; 
						}
						else if (opt == 0) {
							// exit loop when long option sets a flag
							optind = temp; 
							break;
						} else {
							optind = temp; 
							command_args[count] = argv[optind++];  
							count++;
						}
                    }
                    // when there is no more arguments to parse through
				    else 
					    break; 
                }
				
				// continue looking at the next part of the command
				int next_command;
				
                // look for other commands
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
					int i;
					for(i = 0; i < count; i++) {
						fprintf(stdout, "%s ", command_args[i]);
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
					for(i = 0; i < table_index; i++) {
						// close file descriptor
						close(fd_table[i]); 
					}
					if(execvp(command, command_args)) { 
						perror("execvp"); 
					}
				}
				else if (pid > 0) {
					// parent process
					// store child process' info to child_arr 
					child_arr[ch_index].child_pid = pid; 
					int i; 
					for(i = 0; i < count; i++) {
						child_arr[ch_index].command_argv[i] = command_args[i];
					}
					ch_index++; 
				}
				break; 	
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
	return 0; 
}
