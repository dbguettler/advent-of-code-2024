#include "map.h"
#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int keyval_compare(const void *a, const void *b) {
  KeyVal *kv_a = *((KeyVal **)a);
  KeyVal *kv_b = *((KeyVal **)b);
  return kv_a->compare_fn(kv_a->key, kv_b->key);
}

/**
 * Create an empty map.
 */
Map *map_create(int (*compare)(const void *, const void *)) {
  Map *m = (Map *)malloc(sizeof(Map));
  m->compare_fn = compare;
  m->list = list_create();
  return m;
}

void map_destroy(Map *m) {
  while (m->list->len > 0) {
    KeyVal *kv = (KeyVal *)list_remove(m->list, 0);
    free(kv->key);
    free(kv->val);
    free(kv);
  }

  list_destroy(m->list);
  free(m);
}

void map_insert(Map *m, void *key, void *val) {
  if (map_contains(m, key)) {
    fprintf(stderr, "Error: attempted to insert map element with key that "
                    "already exists.\n");
    exit(EXIT_FAILURE);
  }

  KeyVal *kv = (KeyVal *)malloc(sizeof(KeyVal));
  kv->key = key;
  kv->val = val;
  kv->compare_fn = m->compare_fn;
  list_append(m->list, kv);
  qsort(m->list->data, m->list->len, sizeof(void *), keyval_compare);
}

bool map_contains(Map *m, void *key) {
  int left = 0;
  int right = m->list->len - 1;
  int mid = 0;
  while (left <= right) {
    mid = (left + right) / 2;
    if (m->compare_fn(((KeyVal *)m->list->data[mid])->key, key) < 0) {
      left = mid + 1;
    } else if (m->compare_fn(((KeyVal *)m->list->data[mid])->key, key) > 0) {
      right = mid - 1;
    } else {
      return true;
    }
  }

  return false;
}

void *map_get(Map *m, void *key) {
  int left = 0;
  int right = m->list->len - 1;
  int mid = 0;
  while (left <= right) {
    mid = (left + right) / 2;
    if (m->compare_fn(((KeyVal *)m->list->data[mid])->key, key) < 0) {
      left = mid + 1;
    } else if (m->compare_fn(((KeyVal *)m->list->data[mid])->key, key) > 0) {
      right = mid - 1;
    } else {
      return ((KeyVal *)m->list->data[mid])->val;
    }
  }

  fprintf(stderr, "Error: could not find value for key in map.\n");
  exit(EXIT_FAILURE);
}

void **map_remove(Map *m, void *key) {
  int left = 0;
  int right = m->list->len - 1;
  int mid = 0;
  while (left <= right) {
    mid = (left + right) / 2;
    if (m->compare_fn(((KeyVal *)m->list->data[mid])->key, key) < 0) {
      left = mid + 1;
    } else if (m->compare_fn(((KeyVal *)m->list->data[mid])->key, key) > 0) {
      right = mid - 1;
    } else {
      void *k = ((KeyVal *)m->list->data[mid])->key;
      void *v = ((KeyVal *)m->list->data[mid])->val;
      void **kv_ptrs = (void **)malloc(sizeof(void *) * 2);
      kv_ptrs[0] = k;
      kv_ptrs[1] = v;
      return kv_ptrs;
    }
  }

  fprintf(stderr, "Error: could not find value for key in map.\n");
  exit(EXIT_FAILURE);
}