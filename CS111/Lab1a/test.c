#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <ctype.h>

#define TABLE_SIZE 1000;

// flag variables
int verbose_flag = 0;
int command_flag = 0;

// index variables
int current_index = 0;
int table_index = 0;

// in lab1a, we will be implementing just the options rdonly, wronly, command, and verbose
static struct option long_options[] = {
/*file-opening flags*/

/*file-opening options*/
	{"rdonly",	required_argument,	0,				'r'},
	{"wronly",	required_argument,	0,				'w'},

/*subcommand options*/
	{"command",	no_argument,		0,				'c'},

/*miscellaneous options*/
	{"verbose",	no_argument,		&verbose_flag,	 1 },
	{0, 0, 0, 0}
};

typedef struct pidinfo {
	pid_t c_pid; 
	char* cmd_argv[10];
} pidinfo_t;

int main (int argc, char **argv) {
	int table_size = TABLE_SIZE;
	// create a file descriptor table 
	int* fd_table = (int*)malloc(table_size * sizeof(int));
	int fd;
	char* file;
	int opt;
	int opt_index = 0;				// take this to track the order of option used for verbose
	struct option current_index;	// to get the current fd index for verbose

	// Variables to parse the --command option with 
	int in_fd, out_fd, err_fd; 		// stores the in, out, and err arguments
	char* cmd;	// stores the cmd and its arg
	char* cmd_args[10];
	int arg_cnt; // keep track of how many args in --command 
	pidinfo_t child_pa[1000]; 
	int pa_index = 0; // start off with 0 child processes 
	int max_exit_status = 0; // in case program exits in unusual way, exit with this status
	
	// Variables to keep track of where we are in getopt_long
	int optchar; // stores value returned by getopt_long

	if (!fd_table) {
		fprintf(stderr, "couldn't allocate space for file descriptor table, error!");
		exit(1);
	}

	while(1) {
		opt = getopt_long(argc, argv, "", long_options, &opt_index);
		if(opt == -1)
			break;

		switch(opt){
			// option rdonly flag		
			case 'r':
				file = optarg;
				// open the file f for reading only
				fd = open(file, O_RDONLY, S_IRUSR | S_IWUSR | S_IROTH);
				// when open is successful
				if (fd < 0) {
					perror(file);
					exit(0);
				} else if (fd >= 0) {
					fd_table[table_index] = fd;
					table_index++;
				}
				break;
			// option wronly flag
			case 'w':
				file = optarg;
				// open the file for writing only
				fd = open(file, O_WRONLY, S_IRUSR | S_IWUSR | S_IROTH);
				if (fd < 0) {
					perror(file);
					exit(0);
				} else if (fd >= 0) {
					fd_table[table_index] = fd;
					table_index++;
				}
				break;
			// option command
			case 'c':
				// for the command option, will be using the argc from command lines

				// execute a shell command 
				// the first 3 command-line args should be input, output, error streams
				// that refer to logical file descriptors 
				
				// input fd 
				if (optind < argc){
					in_fd = atoi(argv[optind-1]);
					if (in_fd >= table_index){
						fprintf(stderr, "Invalid standard input file descriptor specified. Continuing.\n");
						// exit(1); 
					}
			
					// printf("%d\n", in_fd); 
				}
				// output fd 
				if (optind < argc){
					out_fd = atoi(argv[optind++]); 
					if (out_fd >= table_index){
						fprintf(stderr, "Invalid standard output file descriptor specified. Continuing.\n");
						// exit(1); 
					}
					// printf("%d\n", out_fd); 
				}
				// error fd 
				if (optind < argc){
					err_fd = atoi(argv[optind++]);
					if (err_fd >= table_index){
						fprintf(stderr, "Invalid standard error file descriptor specified. Continuing.\n");
						// exit(1); 
					}
					// printf("%d\n", err_fd); 
				}
				// cmd to be executed 
				if (optind < argc){
					cmd = argv[optind++]; 
					cmd_args[arg_cnt] = cmd; 
					arg_cnt++; 
					// printf("%s\n", cmd); 
				}
				
				// Parsing command-line args 
				int offset = 0; 
				while(1){
					if(optind < argc){
						int temp = optind;
						int len = strlen(argv[optind]); 
						optchar = getopt_long(argc, argv, ":", long_options, &opt_index);
						if(isalpha(optchar)){ 
							// still another option to be parsed
							offset = optind - temp; 
							optind = temp; 
							break; 
						}
						else if (optchar == 0){
							// We hit a long option that set a flag, exit this loop 
							optind = temp; 
							break;
						}
						else if (optchar == '?'){
							if (len < 3){
								cmd_args[arg_cnt] = argv[optind-1];  
								arg_cnt++;
							}
							else{
								cmd_args[arg_cnt] = argv[optind];  
								arg_cnt++;
								break; 
							}
						}
						else{
							// 
							optind = temp; 
							cmd_args[arg_cnt] = argv[optind++];  
							arg_cnt++;
						}
					}
					// no arguments left to parse
					else 
						// optind is greater than/equal to argc 
						break; 
				}
				
				// This portion of code handles commands one after another 
				int next_pos;
				// Handle extra commands 
				current_index = long_options[opt_index]; 
				if (current_index.has_arg == no_argument)
					next_pos = optind + offset - 1;
				if (current_index.has_arg == required_argument)
					next_pos = optind + offset - 2; 
				for(; optind < next_pos; optind++)
					cmd_args[arg_cnt++] = argv[optind];
					
				// cmd_arg array passed to execvp must be terminated by NULL 
				cmd_args[arg_cnt] = NULL;
				
				// Handle verbose flag, if set
				if (verbose_flag){
					fprintf(stdout, "--command %d %d %d ", in_fd, out_fd, err_fd); 
					int j;
					for(j=0; j<arg_cnt; j++)
					// Not sure why this works
						fprintf(stdout, "%s ", cmd_args[j]); 
					printf("\n"); 
				}	
				
				// Check if this command is dealing with pipes.. If it is, we need to deal with it 
				// Assign fds given to --command to existing in, out, err streams (0, 1, 2) 
			
				pid_t pid = fork(); 
				if(pid == 0){ 
					dup2(fd_table[in_fd], 0); 
					dup2(fd_table[out_fd], 1); 
					dup2(fd_table[err_fd], 2); 
					// Close unused ends of pipes 
					/*
					printf("%d\n", desc_is_pipe[in_fd]); 
					if(desc_is_pipe[in_fd] == 1){
						close(file_desc_table[in_fd]); 
						close(file_desc_table[in_fd + 1]); 
					}
					if(desc_is_pipe[out_fd]){
						close(file_desc_table[out_fd]); 
						close(file_desc_table[out_fd - 1]); 
					}
					*/
					// Close all descriptors 
					int i; 
					for(i=0; i < table_index; i++){
						// Possibly closing file descriptors twice
						close(fd_table[i]); 
					}
					if(execvp(cmd, cmd_args)){ 
						perror("execvp: "); 
						// exit(1); 
					}
				}
				else if(pid > 0){					
					// Store info about child process into child_pa array 
					child_pa[pa_index].c_pid = pid; 
					int j; 
					for(j=0; j<arg_cnt; j++)
						child_pa[pa_index].cmd_argv[j] = cmd_args[j];
					pa_index++; 
				}
				else {
					// fork failed
					perror("fork: "); 
					exit(1); 
				}
					
				break; 	
		}
	
		// option verbose flag
		if(verbose_flag) {
			current_index = long_options[opt_index]; // get the current option

			int check_command = strcmp(current_index.name, "command");
			if (current_index.has_arg != required_argument && check_command) {
				printf("--%s",current_index.name);
				printf("%s\n",optarg);
			}

			int check_verbose = strcmp(current_index.name, "verbose");
			if (current_index.has_arg != required_argument && check_verbose) {
				printf("--%s\n",current_index.name);
			}		
		}
	}

	int i;
	// clean up
	for (i = 0; i < table_index; i++) {
		close(fd_table[i]);
	}
	free(fd_table);
	return 0;
}
