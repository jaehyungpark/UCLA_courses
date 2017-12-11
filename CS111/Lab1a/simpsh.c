#include <stdio.h> 
#include <stdlib.h>
#include <getopt.h> 
#include <unistd.h> 	
#include <fcntl.h> 	
#include <errno.h> 
#include <signal.h>
#include <ctype.h>
#include <string.h> 

#define TABLE_SIZE 100 

int verbose_flag = 0; 

// Table variables 
int table_size = TABLE_SIZE; 
int table_index = 0; 

static struct option long_options[] = {
	{"rdonly",		required_argument,		0,				'r'}, 
	{"wronly",		required_argument,		0,				'w'}, 
	{"command",		required_argument,		0,				'c'}, 
	{"verbose",		no_argument, 			&verbose_flag,	 1 },
	{0, 0, 0, 0}
};

typedef struct pidinfo {
	pid_t c_pid; 
	char* cmd_argv[10];
} pidinfo_t;
	
int main(int argc, char** argv){

	// File descriptor table 
	int* fd_table = (int*)malloc(table_size*sizeof(int)); 	// Stores logical file descriptors 
	if (!fd_table) {
		fprintf(stderr, "failed to allocated memory for file descriptor table"); 
		exit(1); 
	}
	int fd; 
	char* file;

	int in_fd, out_fd, err_fd; 		// stores the in, out, and err arguments
	char* cmd;	// stores the cmd and its arg
	char* cmd_args[10];
	int arg_cnt; // keep track of how many args in --command 
	pidinfo_t child_pa[1000]; 
	int pa_index = 0; // start off with 0 child processes 
	
	// Variables to keep track of where we are in getopt_long
	int opt; // stores value returned by getopt_long
	int long_index = 0; // stores index of current option in long_options array 
	struct option current_longopt;
	/*********************
	Parse command-line args 
	**********************/ 
	while(1) { 
		
		opt = getopt_long(argc, argv, ":", long_options, &long_index);  
		if (opt == -1)
			break; 
		// printf("Current optind is: %d\n", optind); 
		if(verbose_flag) {

			current_longopt = long_options[long_index]; 
			// Options that require an argument, except --command 
			int not_command = strcmp(current_longopt.name, "command"); // will return non-zero value if it is not command  
			if (current_longopt.has_arg == required_argument && not_command) {
				printf("--%s ", current_longopt.name); 
				printf("%s\n", optarg); 
			}
			
			// Options that do not require an argument go here 
			int not_verbose = strcmp(current_longopt.name, "verbose");  // will return non-zero value if it is not verbose 
			if (current_longopt.has_arg == no_argument && not_verbose) {
				printf("--%s\n", current_longopt.name); 
			}
		}

		arg_cnt = 0; 
		switch(opt){
			case 0: 
				// getopt_long() set a flag 
				break; 
			case 'r': 
				file = optarg;
				// open a file for reading; store real fd into descriptor table 
				fd = open(file, O_RDONLY, 0644); 
				// open only successful if fd returned is not -1 
				if(fd < 0){
					// failed to open file 
					perror(file);
					exit(0); 
				}
				if(fd >= 0){
					fd_table[table_index] = fd;
					table_index++;
				}
				// clear the file option flags for the next file, if any 
				break; 
				// Check for errors 
			case 'w': 
				file = optarg;
				// open a file for reading; store real fd into descriptor table 
				fd = open(file, O_WRONLY, 0644); 
				if(fd < 0){
					// failed to open file 
					perror(file);
					exit(0);
				}
				if(fd >= 0){
					fd_table[table_index] = fd;
					table_index++;
				}
				// clear the file option flags for the next file, if any 
				break; 
				// Check for errors 
			case 'c':{
				// execute a shell command 
				// the first 3 command-line args should be input, output, error streams
				// that refer to logical file descriptors 
				
				/*
				if (optind <argc) {
					in_fd = atoi(argv[optind-1]);
					out_fd = atoi(argv[optind++]);
					err_fd = atoi(argv[optind++]);

					if (in_fd >= table_index){
						fprintf(stderr, "Invalid standard input file descriptor specified. Continuing.\n");
					}
					if (out_fd >= table_index){
						fprintf(stderr, "Invalid standard output file descriptor specified. Continuing.\n");
					}
					if (err_fd >= table_index){
						fprintf(stderr, "Invalid standard error file descriptor specified. Continuing.\n");
					}
				}
*/
				
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
						opt = getopt_long(argc, argv, ":", long_options, &long_index);
						if(isalpha(opt)){ 
							// still another option to be parsed
							offset = optind - temp; 
							optind = temp; 
							break; 
						}
						else if (opt == 0){
							// We hit a long option that set a flag, exit this loop 
							optind = temp; 
							break;
						}
						else if (opt == '?'){
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
				current_longopt = long_options[long_index]; 
				if (current_longopt.has_arg == no_argument)
					next_pos = optind + offset - 1;
				if (current_longopt.has_arg == required_argument)
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

					// Close all descriptors 
					int i; 
					for(i = 0; i < table_index; i++) {
						// Possibly closing file descriptors twice
						close(fd_table[i]); 
					}
					if(execvp(cmd, cmd_args)){ 
						perror("execvp: "); 
					}
				}
				else if (pid > 0) {
					// parent process
					// Store info about child process into child_pa array 
					child_pa[pa_index].c_pid = pid; 
					int i; 
					for(i = 0; i < arg_cnt; i++)
						child_pa[pa_index].cmd_argv[i] = cmd_args[i];
					pa_index++; 
				}
				else {
					// fork failed
					perror("fork: "); 
					exit(1); 
				}
				
				break; 	
			}
		}
	}
		
	int i; 
	for(i = 0; i < table_index; i++) {
		close(fd_table[i]); 
	}
	free(fd_table); 
	
	return 0; 
}
