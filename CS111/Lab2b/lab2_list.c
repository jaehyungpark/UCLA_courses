// CS 111 lab 2b
// Jaehyung park
// 504212821

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <time.h>
#include "SortedList.h"

#define KEY_SIZE 10
// typedef enum {false=0, true=1} bool;
SortedList_t* list;
SortedListElement_t* elements;

// variables
int opt_yield = 0;
int n_lists = 1;
int n_elements;
char** keys;

bool pass = true;
long long mutex_timer = 0;

// check for corruption, when triggered, return pass to false
void check_corruption() {
	pass = false;
	exit(1);
}

int hash_func(const char str[]) {
	int result = 0;
	for (int i = 0; str[i] != '\0'; i++)
		result += (int)str[i];
	return result;
}

void* malloc_wrapper(size_t size) {
	void* ptr = malloc(size);
	if (ptr == NULL) {
		perror("malloc failed");
		exit(1);
	}
	return ptr;
}

// create random elements
void create_elements(int threads, int iterations) {
	n_elements = threads * iterations;
	elements = malloc_wrapper(n_elements * sizeof(SortedListElement_t));
	keys = malloc_wrapper(n_elements * sizeof(char*));
	srand(time(NULL));
	for (int i = 0; i < n_elements; i++) {
		elements[i].prev = elements[i].next = NULL;
		keys[i] = malloc_wrapper(KEY_SIZE + 1);
		for (int j = 0; j < KEY_SIZE; j++) {
			keys[i][j] = 'a' + rand() % 26;
		}
		keys[i][KEY_SIZE] = '\0';
		elements[i].key = keys[i];
	}
}

struct timespec get_time() {
	struct timespec time;
	if (clock_gettime(CLOCK_MONOTONIC, &time) == -1) {
		perror("get_time failed");
		exit(1);
	}
	return time;
}

struct timespec time_diff(const struct timespec* start, const struct timespec* end) {
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

// assign lock as PTHREAD_MUTEX_INITIALIZER
pthread_mutex_t* lock_mutex;
bool* lock_spin;

void SortedList_insert_mutex(SortedList_t* list, SortedListElement_t* element) {
	int num = hash_func(element->key) % n_lists;
	struct timespec start_time = get_time();
	// lock
	if (pthread_mutex_lock(&lock_mutex[num]) != 0) {
		perror("mutex lock failed");
		exit(1);
	}
	struct timespec end_time = get_time();
	struct timespec diff = time_diff(&start_time, &end_time);
	mutex_timer += time_ns(&diff);
	SortedList_insert(list, element);
	if (pthread_mutex_unlock(&lock_mutex[num]) != 0) {
		perror("mutex unlock failed");
		exit(1);
	}
}

void SortedList_insert_spin(SortedList_t* list, SortedListElement_t* element) {
	int num = hash_func(element->key) % n_lists;
	while (__sync_lock_test_and_set(&lock_spin[num], true));
	SortedList_insert(list, element);
	__sync_lock_release(&lock_spin[num]);
}

int SortedList_length_mutex(SortedList_t* l) {
	int num = l - list;
	struct timespec start_time = get_time();
	if (pthread_mutex_lock(&lock_mutex[num]) != 0) {
		perror("mutex lock failed");
		exit(1);
	}
	struct timespec end_time = get_time();
	struct timespec diff = time_diff(&start_time, &end_time);
	mutex_timer += time_ns(&diff);
	int len = SortedList_length(l);
	if (pthread_mutex_unlock(&lock_mutex[num]) != 0) {
		perror("mutex unlock failed");
		exit(1);
	}
	return len;
}

int SortedList_length_spin(SortedList_t* l) {
	int num = l - list;
	while (__sync_lock_test_and_set(&lock_spin[num], true));
	int len = SortedList_length(l);
	__sync_lock_release(&lock_spin[num]);
	return len;
}

void SortedList_ild_default(SortedListElement_t* element) {
	int num = hash_func(element->key) % n_lists;
	if (SortedList_lookup(&list[num], element->key) != element) {
		fprintf(stderr, "corruption detected\n");
		check_corruption();
	}
	SortedList_delete(element);
}

void SortedList_ild_mutex(SortedListElement_t* element) {
	int num = hash_func(element->key) % n_lists;
	struct timespec start_time = get_time();
	if (pthread_mutex_lock(&lock_mutex[num]) != 0) {
		perror("mutex lock failed");
		exit(1);
	}
	struct timespec end_time = get_time();
	struct timespec diff = time_diff(&start_time, &end_time);
	mutex_timer += time_ns(&diff);
	SortedList_ild_default(element);
	if (pthread_mutex_unlock(&lock_mutex[num]) != 0) {
		perror("mutex unlock failed");
		exit(1);
	}
}

void SortedList_ild_spin(SortedListElement_t* element) {
	int num = hash_func(element->key) % n_lists;
	while (__sync_lock_test_and_set(&lock_spin[num], true));
	SortedList_ild_default(element);
	__sync_lock_release(&lock_spin[num]);
}

// create wrapper
void (*insert_wrapper)(SortedList_t*, SortedListElement_t*) = SortedList_insert;
void (*ld_wrapper)(SortedListElement_t*) = SortedList_ild_default;
int (*length_wrapper)(SortedList_t*) = SortedList_length;

struct thread_args {
	int thread_nums;
	int iterations;
};
pthread_t* thread_ids;

void join_threads(int threads) {
	for (int thread_nums = 0; thread_nums < threads; thread_nums++) {
		if (pthread_join(thread_ids[thread_nums], NULL) != 0) {
			fprintf(stderr, "failed to join pthreads\n");
			exit(1);
		}
	}
	free(thread_ids);
}

void* threads_func(void* arg) {
	int thread_nums = ((struct thread_args*)arg)->thread_nums, iterations = ((struct thread_args*)arg)->iterations;
	for (int i = 0; i < iterations; i++) {
		SortedListElement_t* element = elements + thread_nums * iterations + i;
		int num = hash_func(element->key) % n_lists;
		insert_wrapper(&list[num], element);
	}
	for (int i = 0; i < n_lists; i++) {
		length_wrapper(&list[i]);
	}
	for (int i = 0; i < iterations; i++) {
		ld_wrapper(elements + thread_nums * iterations + i);
	}
	free(arg);
	pthread_exit(NULL);
}

void create_threads(int threads, int iterations) {
	thread_ids = malloc_wrapper(threads * sizeof(pthread_t));
	for (int thread_nums = 0; thread_nums < threads; thread_nums++) {
		struct thread_args* arg = malloc_wrapper(sizeof(struct thread_args));
		arg->thread_nums = thread_nums, arg->iterations = iterations;
		if (pthread_create(&thread_ids[thread_nums], NULL, threads_func, (void*)arg) != 0) {
			fprintf(stderr, "failed to create pthread\n");
			exit(1);
		}
	}
}

// main
int main(int argc, char* argv[]) {
	bool readable = false;
	int iterations = 1;
	int threads = 1;
	char yieldopts[] = "none";
	char syncopts[] = "none";
	
	struct option long_options[] = {
		{"correct",		no_argument,			NULL,	'c'},
		{"readable",	no_argument,			NULL,	'r'},
		{"iterations",	required_argument,		NULL,	'i'},
		{"lists",		required_argument,		NULL,	'l'},
		{"sync",		required_argument,		NULL,	's'},
		{"threads",		required_argument,		NULL,	't'},
		{"yield",		required_argument,		NULL,	'y'}
	};
	char c; 
	while ((c = getopt_long(argc, argv, "", long_options, NULL)) != -1) switch (c) {
		case 'c':
			break;
		case 'r':
			readable = true;
			break;
		case 'i':
			iterations = atoi(optarg);
			break;
		case 'l':
			n_lists = atoi(optarg);
			break;
		case 's':
			strcpy(syncopts, optarg);
			switch (optarg[0]) {
				case 'm':
					insert_wrapper = SortedList_insert_mutex;
					ld_wrapper = SortedList_ild_mutex;
					length_wrapper = SortedList_length_mutex;
					break;
				case 's':
					insert_wrapper = SortedList_insert_spin;
					ld_wrapper = SortedList_ild_spin;
					length_wrapper = SortedList_length_spin;
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
				case 'i':
					opt_yield |= INSERT_YIELD;
					break;
				case 'd':
					opt_yield |= DELETE_YIELD;
					break;
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

	list = malloc_wrapper(n_lists * sizeof(SortedList_t));
	lock_mutex = malloc_wrapper(n_lists * sizeof(pthread_mutex_t));
	lock_spin = malloc_wrapper(n_lists * sizeof(bool));

	for (int i = 0; i < n_lists; i++) {
		list[i].prev = list[i].next = &list[i], list[i].key = NULL;
		pthread_mutex_init(&lock_mutex[i], NULL);
		lock_spin[i] = false;
	}

	create_elements(threads, iterations);
	struct timespec start_time = get_time();
	create_threads(threads, iterations);
	join_threads(threads);
	struct timespec end_time = get_time();

	for (int i = 0; i < n_lists; i++) {
		if (SortedList_length(&list[i]) != 0) {
			fprintf(stderr, "corruption detected!\n");
			check_corruption();
		}
	}
	
	// free all used pointers
	free(elements);
	for (int i = 0; i < n_elements; i++) {
		free(keys[i]);
	}
	free(keys);
	free(lock_spin);
	free(lock_mutex);
	free(list);
	
	struct timespec diff = time_diff(&start_time, &end_time);
	long long op_time = time_ns(&diff);

	// Print the results, results depends on if it passed the test or not
	// if pass, get stdout, if not, get stderr (since it didn't pass)
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
		fprintf(print_output, "Average time per operation: %lld ns\n", op_time/operations);
		if (syncopts[0] == 'm') {
			fprintf(print_output, "Average time for wait lock: %lld ns\n", mutex_timer/operations);
		}
	} else {
		if (syncopts[0] == 'm') {
			fprintf(print_output, "list-%s-%s,%d,%d,%d,%d,%lld,%lld,%lld\n", yieldopts, syncopts, threads, 
				iterations, n_lists, operations, op_time, op_time/operations, mutex_timer/operations);
		} else {
			fprintf(print_output, "list-%s-%s,%d,%d,%d,%d,%lld,%lld\n", yieldopts, syncopts, threads, iterations, 
				n_lists, operations, op_time, op_time/operations);
		}
	}
	// if pass, return 0
	if (pass) {
		exit(0);
	} else {
		exit(1);
	}
}
