#include "list.h"
#include "aoc_input.h"
#include "strings.h"
#include <stdlib.h>
#include <stdio.h>

int comparison(const void * a, const void * b) {
  int * num_a = (int*) a;
  int * num_b = (int*) b;
  return (*num_a - *num_b);
}

void part1() {
  List * file_lines = get_lines("inputs/day1.txt");
  int count = file_lines->len;
  int first[file_lines->len];
  int second[file_lines->len];

  for (int i = 0; i < file_lines->len; i++) {
    List * split_line = str_split(file_lines->data[i], "   ");
    if (split_line->len != 2) {
      printf("Error splitting lines. Expected 2 numbers but got %d\n", split_line->len);
      exit(1);
    }
    first[i] = atoi(split_line->data[0]);
    second[i] = atoi(split_line->data[1]);
    free(split_line->data[0]);
    free(split_line->data[1]);
    free(file_lines->data[i]);
    list_destroy(split_line);
  }

  list_destroy(file_lines);

  qsort(first, count, sizeof(int), comparison);
  qsort(second, count, sizeof(int), comparison);

  int sum = 0;

  for (int i = 0; i < count; i++) {
    int val = first[i] - second[i];
    if (val < 0) {
      val *= -1;
    }
    sum += val;
  }

  printf("Sum: %d\n", sum);
}

void part2() {
  List * file_lines = get_lines("inputs/day1.txt");
  int count = file_lines->len;
  int first[file_lines->len];
  int second[file_lines->len];

  for (int i = 0; i < file_lines->len; i++) {
    List * split_line = str_split(file_lines->data[i], "   ");
    if (split_line->len != 2) {
      printf("Error splitting lines. Expected 2 numbers but got %d\n", split_line->len);
      exit(1);
    }
    first[i] = atoi(split_line->data[0]);
    second[i] = atoi(split_line->data[1]);
    free(split_line->data[0]);
    free(split_line->data[1]);
    free(file_lines->data[i]);
    list_destroy(split_line);
  }

  list_destroy(file_lines);

  qsort(first, count, sizeof(int), comparison);
  qsort(second, count, sizeof(int), comparison);

  int score = 0;
  int pos = 0;

  for (int i = 0; i < count && pos < count; i++) {
    int instances = 0;
    while (second[pos] < first[i]) {
      pos++;
    }

    while (second[pos] == first[i]) {
      instances++;
      pos++;
    }
    score += first[i] * instances;

    for (int j = i + 1; first[j] == first[i]; j++) {
      score += first[i] * instances;
      i++;
    }
  }

  printf("Score: %d\n", score);
}

int main() {
  part1();
  part2();
  exit(0);
}