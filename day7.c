#include "aoc_input.h"
#include "list.h"
#include "strings.h"
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Check if the equation fragment is solvable with multiplication and addition.
 * This function will ignore the first value in the equation and will instead
 * use current_val. It will also ignore any operands in the equation with an
 * index greater than last_idx.
 */
bool is_solvable(List *equation, long current_val, int last_idx,
                       bool is_part_2) {
  if (last_idx < 1) {
    return false;
  }
  if (last_idx == 1) {
    return current_val == *((long *)equation->data[1]);
  }

  long last_val = *((long *)equation->data[last_idx]);

  bool multiplication_solvable = false;
  if (current_val % last_val == 0) {
    multiplication_solvable =
        is_solvable(equation, current_val / last_val, last_idx - 1, is_part_2);
  }

  if (multiplication_solvable) {
    return true;
  }

  if (is_part_2) {
    bool concatenation_solvable = false;
    long unconcatenated = current_val - last_val;
    int num_digits = (int)log10(last_val) + 1;
    if (unconcatenated % (int)pow(10, num_digits) == 0) {
      concatenation_solvable = is_solvable(
          equation, unconcatenated / (int)pow(10, num_digits), last_idx - 1, is_part_2);
    }

    if (concatenation_solvable) {
      return true;
    }
  }

  return current_val > last_val &&
         is_solvable(equation, current_val - last_val, last_idx - 1, is_part_2);
}

void parts_1_and_2() {
  List *file_lines = get_lines("inputs/day7.txt");

  if (file_lines == NULL) {
    fprintf(stderr, "Could not read input file.");
    exit(EXIT_FAILURE);
  }

  List *equations[file_lines->len];
  int num_equations = file_lines->len;

  for (int i = 0; i < file_lines->len; i++) {
    equations[i] = list_create();
    List *nums = str_split(file_lines->data[i], ": ");

    while (nums->len > 0) {
      long *num = (long *)malloc(sizeof(long));
      char **endptr = NULL;
      *num = strtol(nums->data[0], endptr, 10);
      list_append(equations[i], num);
      free(list_remove(nums, 0));
    }

    free(file_lines->data[i]);
    list_destroy(nums);
  }
  list_destroy(file_lines);

  // We now have a list of all the (possibly solvable) equations.
  long sum_part_1 = 0;
  long sum_part_2 = 0;
  for (int i = 0; i < num_equations; i++) {
    if (is_solvable(equations[i], *((long *)equations[i]->data[0]),
                          equations[i]->len - 1, false)) {
      sum_part_1 += *((long *)equations[i]->data[0]);
    }
    if (is_solvable(equations[i], *((long *)equations[i]->data[0]),
                          equations[i]->len - 1, true)) {
      sum_part_2 += *((long *)equations[i]->data[0]);
    }
  }

  printf("Part 1: %ld\n", sum_part_1);
  printf("Part 2: %ld\n", sum_part_2);

  // Cleanup
  for (int i = 0; i < num_equations; i++) {
    while (equations[i]->len > 0) {
      free(list_remove(equations[i], 0));
    }
    list_destroy(equations[i]);
  }
}

int main() {
  parts_1_and_2();
  exit(EXIT_SUCCESS);
}