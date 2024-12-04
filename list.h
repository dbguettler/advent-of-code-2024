#ifndef _LIST_H_
#define _LIST_H_

#include <ctype.h>

typedef struct list_struct {
	void ** data;
	int len;
} List;

List * list_create();

void list_destroy(List * l);

void list_append(List * l, void * item);

#endif
