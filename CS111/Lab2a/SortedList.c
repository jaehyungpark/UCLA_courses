// CS 111 lab 2a
// Jaehyung park
// 504212821

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sched.h>
#include "SortedList.h"
extern void check_corruption();

// created an additional function to insert between the list
int insert_between(SortedListElement_t* element, SortedListElement_t* prev, SortedListElement_t* next) {
	// check for validity, if element, prev, and next are null and is not a doubly linked list
	if (element == NULL || prev == NULL || next == NULL || prev->next != next || next->prev != prev) {
		fprintf(stderr, "corruption: linked list is broken\n");
		check_corruption();
		return -1;
	}
	element->prev = prev;
	element->next = next;
	prev->next = element;
	next->prev = element;
	return 0;
}

void SortedList_insert(SortedList_t* list, SortedListElement_t* element) {
	// check if both head of the list or element to be added is null
	if (list == NULL || element == NULL) {
		fprintf(stderr, "invalid input\n");
		check_corruption();
		return;
	}
	// create current node called curr
	SortedListElement_t* curr;
	for (curr = list->next; curr != NULL && curr != list; curr = curr->next) {
		if (strcmp(curr->key, element->key) >= 0) {
			break;
		}
	}
	// insert if opt_yield & INSERT_YEILD
	if (opt_yield & INSERT_YIELD) {
		sched_yield();
	}
	insert_between(element, curr->prev, curr);
}

int SortedList_delete(SortedListElement_t* element) {
	// check is element is null
	if (element == NULL) {
		fprintf(stderr, "invalid input\n");
		check_corruption();
		return 1;
	}
	if (element->prev == NULL || element->prev->next != element || element->next == NULL || element->next->prev != element) {
		fprintf(stderr, "corruption: linked list is broken\n");
		check_corruption();
		return 1;
	}
	// delete if opt_yield & DELETE_YIELD
	if (opt_yield & DELETE_YIELD) {
		sched_yield();
	}
	element->prev->next = element->next;
	element->next->prev = element->prev;
	return 0;
}

SortedListElement_t* SortedList_lookup(SortedList_t* list, const char* key) {
	// check if both list or key is null
	if (list == NULL || key == NULL) {
		fprintf(stderr, "invalid input\n");
		check_corruption();
		return NULL;
	}
	SortedListElement_t* curr;
	for (curr = list->next; curr != NULL && curr != list; curr = curr->next) {
		if (strcmp(curr->key, key) == 0) {
			return curr;
		}
		// lookup if opt_yield & LOOKUP_YIELD
		else if (opt_yield & LOOKUP_YIELD) {
			sched_yield();
		}
	}
	return NULL;
}

int SortedList_length(SortedList_t *list) {
	// check if list is null
	if (list == NULL) {
		fprintf(stderr, "invalid input\n");
		check_corruption();
		return -1;
	}
	int m_count = 0;
	SortedListElement_t* curr;
	for (curr = list->next; curr != list; curr = curr->next) {
		if (curr == NULL || curr->next == NULL || curr->next->prev != curr) {
		  // simultaneously check curr's next and curr->next's prev
			fprintf(stderr, "corruption: linked list is broken\n");
			check_corruption();
			return -1;  // return -1 if the list is corrupted
		}
		// length if opt_yield & LOOKUP_YIELD
		else if (opt_yield & LOOKUP_YIELD) {
			sched_yield();
		}
		m_count++;
	}
	return m_count;
}