#ifndef _LIST_H_
#define _LIST_H_

#include <ctype.h>

typedef struct list_struct {
  void **data;
  int len;
} List;

List *list_create();

void list_destroy(List *l);

void list_append(List *l, void *item);

void *list_remove(List *l, int idx);

void list_insert(List *l, void *item, int idx);

#endif
