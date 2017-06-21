// CS 111 lab 2a
// Jaehyung park
// 504212821

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "SortedList.h"

#define KEY_SIZE 10
// typedef enum {false=0, true=1} bool;
SortedList_t list;
SortedListElement_t* elements;

// variables
int opt_yield;
int n_elements;
char** keys;
bool pass = true;

void create_elements(int threads, int iterations) {
	n_elements = threads * iterations;
	elements = malloc(n_elements * sizeof(SortedListElement_t));
	keys = malloc(n_elements * sizeof(char*));
	srand(time(NULL));
	for (int i = 0; i < n_elements; i++) {
		elements[i].prev = elements[i].next = NULL;
		keys[i] = malloc(KEY_SIZE + 1);
		for (int j = 0; j < KEY_SIZE; j++) {
			keys[i][j] = 'a' + rand() % 26;
		}
		keys[i][KEY_SIZE] = '\0';
		elements[i].key = keys[i];
	}
}

// check for corruption, when triggered, return pass to false
void check_corruption() {
	pass = false;
	exit(1);
}

// assign lock as PTHREAD_MUTEX_INITIALIZER
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
bool lock_spin = false;

// get default SortedList_insert from SortedList.h

// mutex lock
void SortedList_insert_mutex(SortedList_t* list, SortedListElement_t* element) {
	// lock
	if (pthread_mutex_lock(&lock) != 0) {
		perror("pthread_mutex_lock() failed");
		exit(1);
	}
	//insert and unlock
	SortedList_insert(list, element);
	if (pthread_mutex_unlock(&lock) != 0) {
		perror("pthread_mutex_unlock() failed");
		exit(1);
	}
}

// spin lock
void SortedList_insert_spin(SortedList_t* list, SortedListElement_t* element) {
	while (__sync_lock_test_and_set(&lock_spin, true));
	// insert and release
	SortedList_insert(list, element);
	__sync_lock_release(&lock_spin);
}

// default for ld option
void SortedList_ld_default(SortedListElement_t* element) {
	if (SortedList_lookup(&list, element->key) != element) {
		fprintf(stderr, "corruption has been detected\n");
		check_corruption();
	}
	SortedList_delete(element);
}

// mutex lock
void SortedList_ld_mutex(SortedListElement_t* element) {
	if (pthread_mutex_lock(&lock) != 0) {
		perror("pthread_mutex_lock() failed");
		exit(1);
	}
	// insert and unlock
	SortedList_ld_default(element);
	if (pthread_mutex_unlock(&lock) != 0) {
		perror("pthread_mutex_unlock() failed");
		exit(1);
	}
}

// spin lock
void SortedList_ld_spin(SortedListElement_t* element) {
	while (__sync_lock_test_and_set(&lock_spin, true));
	SortedList_ld_default(element);
	__sync_lock_release(&lock_spin);
}

// create wrapper for both SortedList_insert and SortedList_ld_default
void (*insert_wrapper)(SortedList_t*, SortedListElement_t*) = SortedList_insert;
void (*ld_wrapper)(SortedListElement_t*) = SortedList_ld_default;

// time
struct timespec get_time() {
	struct timespec time;
	if (clock_gettime(CLOCK_MONOTONIC, &time) == -1) {
		perror("clock_gettime() failed");
		exit(1);
	}
	return time;
}

struct timespec time_diff(const struct timespec* start, const struct timespec* end) {
	struct timespec diff = {
		end->tv_sec - start->tv_sec, 
		end->tv_nsec - start->tv_nsec
	};
	// whenever time is negative
	if (diff.tv_nsec < 0) {
		diff.tv_sec -= 1;
		diff.tv_nsec += 1e9;
	}
    return diff;
}

long long time_ns(const struct timespec* time) {
	return time->tv_sec * 1e9 + time->tv_nsec;
}

// pthreads
pthread_t* thread_ids;

struct thread_arguments {
	int thread_num;
	int iterations;
};

void* function_threads(void* arg) {
	int thread_num = ((struct thread_arguments*)arg)->thread_num, iterations = ((struct thread_arguments*)arg)->iterations;
	for (int i = 0; i < iterations; i++) {
		insert_wrapper(&list, elements + thread_num * iterations + i);
	}
	SortedList_length(&list);
	for (int i = 0; i < iterations; i++) {
		ld_wrapper(elements + thread_num * iterations + i);
	}
	pthread_exit(NULL);
}

void create_threads(int threads, int iterations) {
	thread_ids = malloc(threads * sizeof(pthread_t));
	int thread_num;
	for (thread_num = 0; thread_num < threads; thread_num++) {
		struct thread_arguments* arg = malloc(sizeof(struct thread_arguments));
		arg->thread_num = thread_num, arg->iterations = iterations;
		if (pthread_create(&thread_ids[thread_num], NULL, function_threads, (void*)arg) != 0) {
			fprintf(stderr, "pthread_create() failed\n");
			exit(1);
		}
	}
}

void join_threads(int threads) {
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
	
	// Load options
	int readable = 0;
	int iterations = 1;
	int threads = 1;
	// default yieldopts and syncopts to none
	char yieldopts[] = "none";
	char syncopts[] = "none";
	
	struct option longopts[] = {
		{"correct",		no_argument,		0,	'c'},
		{"readable",	no_argument,		0,	'r'},
		{"iterations",	required_argument,	0,	'i'},
		{"sync",		required_argument,	0,	's'},
		{"threads",		required_argument,	0,	't'},
		{"yield",		required_argument,	0,	'y'}
	};

	char opt; 
	while (1) {
		opt = getopt_long(argc, argv, "", longopts, NULL);
		if (opt == -1) {
			break;
		}
		switch (opt) {
			case 'c':
				// not implemented
				break;
			case 'i':
				iterations = atoi(optarg);
				break;
			case 'r':
				readable = 1;
				break;
			case 's':
				strcpy(syncopts, optarg);
				switch (optarg[0]) {
					// --sync=m
					case 'm':
						insert_wrapper = SortedList_insert_mutex;
						ld_wrapper = SortedList_ld_mutex;
						break;
					// --sync=s
					case 's':
						insert_wrapper = SortedList_insert_spin;
						ld_wrapper = SortedList_ld_spin;
						break;
					default: 
						break;
				}
				break;
			case 't':
				threads = atoi(optarg);
				break;
			case 'y': 
				strcpy(yieldopts, optarg);
				for (int i = 0; optarg[i] != '\0'; i++) switch (optarg[i]) {
					// --yield=i
					case 'i':
						opt_yield |= INSERT_YIELD;
						break;
					// --yield=d
					case 'd':
						opt_yield |= DELETE_YIELD;
						break;
					// --yield=l
					case 'l':
						opt_yield |= LOOKUP_YIELD;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}

	// to get the starting and ending time of creating and joining threads
	list.prev = list.next = &list, list.key = NULL;
	create_elements(threads, iterations);
	struct timespec start_time = get_time();
	create_threads(threads, iterations);
	join_threads(threads);
	struct timespec end_time = get_time();

	if (SortedList_length(&list) != 0) {
		fprintf(stderr, "corruption has been detected\n");
		check_corruption();
	}

	// destroy both elements and keys
	free(elements);
	for (int i = 0; i < n_elements; i++) {
		free(keys[i]);
	}
	free(keys);
	
	struct timespec diff = time_diff(&start_time, &end_time);
	long long run_time = time_ns(&diff);

	// Print the results, results depends on if it passed the test or not
	FILE* print_output;
	if (pass) {
		print_output = stdout;
	} else {
		print_output = stderr;
	}
	// total number of operations performed (threads x iterations x (insert + lookup + delete))
	int operations = threads * iterations * 3;
	if (readable) {
		fprintf(print_output, "Name of the test: list-%s-%s\n", yieldopts, syncopts);
		fprintf(print_output, "Number of threads: %d\n", threads);
		fprintf(print_output, "Number of iterations: %d\n", iterations);
		fprintf(print_output, "Number of lists: %d\n", 1);
		fprintf(print_output, "Number of operations: %d\n", operations);
		fprintf(print_output, "Total run time: %ld.%09ld s\n", diff.tv_sec, diff.tv_nsec);
		fprintf(print_output, "Average time per operation: %lld ns\n", run_time/operations);
	}
	else {
		fprintf(print_output, "list-%s-%s,%d,%d,%d,%d,%lld,%lld\n", yieldopts, syncopts, threads, 
			iterations, 1, operations, run_time, run_time/operations);
	}
	if (pass) {
		exit(0);
	} else {
		exit(1);
	}
}
