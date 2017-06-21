// CS 111 lab 2a
// Jaehyung park
// 504212821

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <sched.h> 

// variables
int opt_yield = 0;
// since every function is passed by a long long value, made a counter with long long type
long long count = 0LL;

// given default add function from the spec in question 2.1.1
void add_default(long long* pointer, long long value) {
	long long sum = *pointer + value;
	if (opt_yield) {
		sched_yield();
	}
	*pointer = sum;
}

// mutex with --sync=m option
void add_mutex(long long* pointer, long long value) {
	// assign lock as PTHREAD_MUTEX_INITIALIZER
	static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	// check for both lock and unlock
	if (pthread_mutex_lock(&lock) != 0) {
		perror("pthread_mutex_lock() failed");
		exit(1);
	}
	*pointer = *pointer + value;
	if (pthread_mutex_unlock(&lock) != 0) {
		perror("pthread_mutex_unlock() failed");
		exit(1);
	}
}

// mutex with --sync=s option
void add_spin(long long* pointer, long long value) {
	static bool lock = false;
	while (__sync_lock_test_and_set(&lock, true));
	*pointer = *pointer + value;
	// release
	__sync_lock_release(&lock);
}

// mutex with --sync=c option
void add_counter(long long* pointer, long long value) {
	long long oldval;
	do {
		oldval = *pointer;
	} while (__sync_val_compare_and_swap(pointer, oldval, oldval + value) != oldval);
}

void (*add)(long long*, long long) = add_default;

// get starting time for the run
struct timespec get_time() {
	struct timespec time;
	if (clock_gettime(CLOCK_MONOTONIC, &time) == -1) {
		perror("clock_gettime() failed");
		exit(1);
	}
	return time;
}

struct timespec get_time_diff(const struct timespec* start, const struct timespec* end) {
	struct timespec diff = {
		end->tv_sec - start->tv_sec, 
		end->tv_nsec - start->tv_nsec
	};
	if (diff.tv_nsec < 0) {
		diff.tv_sec -= 1;
		diff.tv_nsec += 1e9;
	}
    return diff;
}

long long time_ns(const struct timespec* time) {
	return time->tv_sec * 1e9 + time->tv_nsec;
}

// threads
pthread_t* thread_ids;

void* get_threads(void* arg) {
	int iterations = (int)(intptr_t)arg;
	for (int i = 0; i < iterations; i++) {
		add(&count, 1);
	}
	for (int i = 0; i < iterations; i++) {
		add(&count, -1);
	}
	pthread_exit(NULL);
}

void threads_create(int threads, int iterations) {
	thread_ids = malloc(threads * sizeof(pthread_t));
	int thread_num;
	for (thread_num = 0; thread_num < threads; thread_num++) {
		if (pthread_create(&thread_ids[thread_num], NULL, get_threads, (void*)(intptr_t)iterations) != 0) {
			fprintf(stderr, "pthread_create() failed\n");
			exit(1);
		}
	}
}

void threads_join(int threads) {
	int thread_num;
	for (thread_num = 0; thread_num < threads; thread_num++) {
		if (pthread_join(thread_ids[thread_num], NULL) != 0) {
			fprintf(stderr, "pthread_join() failed\n");
			exit(1);
		}
	}
	free(thread_ids);
}

int main(int argc, char* argv[]) {
	// variables
	const char* yieldopts = "";
	// default syncopts as none
	const char* syncopts = "-none";
	int readable = 0;
	// set default iterations and threads to 1
	int iterations = 1;
	int threads = 1;
	long long run_time;

	struct option long_options[] = {
		{"correct",		no_argument,		0,	'c'},
		{"readable",	no_argument,		0,	'r'},
		{"yield",		no_argument,		0,	'y'},
		{"iterations",	required_argument,	0,	'i'},
		{"sync",		required_argument,	0,	's'},
		{"threads",		required_argument,	0,	't'},
	};
	char opt;	
	while (1) {
		opt = getopt_long(argc, argv, "", long_options, NULL);
		if (opt == -1) {
			break;
		}
		switch (opt) {
			case 'c':
				break;
			case 'i':
				iterations = atoi(optarg);
				break;
			case 'r':
				readable = 1;
				break;
			case 's': 
				switch (optarg[0]) {
					// --sync=m
					case 'm':
						syncopts = "-m";
						add = add_mutex;
						break;
					// --sync=s
					case 's':
						syncopts = "-s";
						add = add_spin;
						break;
					// --sync=c
					case 'c':
						syncopts = "-c";
						add = add_counter;
						break;
					default: 
						break;
				}
				break;
			case 't':
				// get number of threads from option input
				threads = atoi(optarg);
				break;
			case 'y':
				yieldopts = "-yield";
				opt_yield = 1;
				break;
			default: 
				break;
		}
	}
	
	// to get the starting and ending time of creating and joining threads
	struct timespec starting_time = get_time();
	threads_create(threads, iterations);
	threads_join(threads);
	struct timespec ending_time = get_time();
	
	// get the time difference
	struct timespec diff = get_time_diff(&starting_time, &ending_time);
	// get run time
	run_time = time_ns(&diff);
	// total number of operations performed (threads x iterations x 2)
	int operations = threads * iterations * 2;
	// print out measurements
	if (readable) {
		printf("Name of the test: add%s%s\n", yieldopts, syncopts);
		printf("Number of threads: %d\n", threads);
		printf("Number of iterations: %d\n", iterations);
		printf("Number of operations: %d\n", operations);
		printf("Total run time: %ld.%09ld s\n", diff.tv_sec, diff.tv_nsec);
		printf("Average time per operation: %lld ns\n", run_time/operations);
		printf("Final counter value: %lld\n", count);
	}
	else {
		printf("add%s%s,%d,%d,%d,%lld,%lld,%lld\n", yieldopts, syncopts, threads, iterations, 
			operations, run_time, run_time/operations, count);
	}
	exit(0);
}
