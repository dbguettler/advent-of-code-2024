#ifndef _MAP_H_
#define _MAP_H_

#include "list.h"
#include <stdbool.h>

typedef struct map_struct {
  List *list;
  int (*compare_fn)(const void *, const void *);
} Map;

typedef struct keyval_struct {
  void *key;
  void *val;
  int (*compare_fn)(const void *, const void *);
} KeyVal;

Map *map_create(int (*compare)(const void *, const void *));

void map_destroy(Map *m);

void map_insert(Map *m, void *key, void *val);

bool map_contains(Map *m, void *key);

void *map_get(Map *m, void *key);

void **map_remove(Map *m, void *key);

#endif
