#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

List * list_create() {
	List * l = (List *) malloc(sizeof(List));
	l->len = 0;
	l->data = NULL;
	return l;
}

/**
 * Free the memory used for the list. The items in the list MUST
 * be freed manually before calling this function.
 * 
 * @param l The list to free
 */
void list_destroy(List * l) {
	free(l->data);
	free(l);
}

void list_append(List * l, void * item) {
	if (l->len == 0) {
		l->data = malloc(sizeof(void *));
		l->data[0] = item;
		l->len = 1;
	} else {
		l->data = realloc(l->data, sizeof(void *) * (l->len + 1));
		l->data[l->len] = item;
		l->len += 1;
	}
}

void * list_remove(List * l, int idx) {
	if (idx >= l->len) {
		fprintf(stderr, "Error: cannot remove item at index %d from list of length %d.\n", idx, l->len);
		exit(EXIT_FAILURE);
	}

	void * return_ptr = l->data[idx];
	for (int i = idx; i < l->len - 1; i++) {
		l->data[i] = l->data[i+1];
	}
	l->data = realloc(l->data, sizeof(void*) * (l->len - 1));
	l->len -= 1;

	return return_ptr;
}

void list_insert(List * l, void * item, int idx) {
	if (idx > l->len) {
		fprintf(stderr, "Error: cannot insert item at index %d into list of length %d.\n", idx, l->len);
		exit(EXIT_FAILURE);
	}

	l->data = realloc(l->data, sizeof(void*) * (l->len + 1));
	l->len += 1;
	
	for (int i = l->len - 1; i > idx; i--) {
		l->data[i] = l->data[i-1];
	}
	l->data[idx] = item;
}