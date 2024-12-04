#include "list.h"
#include <stdlib.h>

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
