#include "aoc_input.h"
#include "list.h"
#include "strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1() {
  List *file_lines = get_lines("inputs/day5.txt");

  List *ordering_instrs = list_create();
  List *updates = list_create();

  bool on_updates = false;
  for (int i = 0; i < file_lines->len; i++) {
    if (!on_updates && strcmp((char *)file_lines->data[i], "") == 0) {
      on_updates = true;
      free(file_lines->data[i]);
      continue;
    }
    if (!on_updates) {
      List *instr = str_split((char *)file_lines->data[i], "|");
      int *nums = (int *)malloc(sizeof(int) * 2);
      nums[0] = atoi(instr->data[0]);
      nums[1] = atoi(instr->data[1]);
      list_append(ordering_instrs, nums);
      free(instr->data[0]);
      free(instr->data[1]);
      list_destroy(instr);
    } else {
      List *instr = str_split((char *)file_lines->data[i], ",");
      List *nums = list_create();
      for (int j = 0; j < instr->len; j++) {
        int *num_ptr = (int *)malloc(sizeof(int));
        *num_ptr = atoi(instr->data[j]);
        printf("%d,", *num_ptr);
        list_append(nums, num_ptr);
        free(instr->data[j]);
      }
      list_append(updates, nums);
      list_destroy(instr);
    }
    free(file_lines->data[i]);
  }

  list_destroy(file_lines);

  for (int i = 0; i < ordering_instrs->len; i++) {
    printf("%d#%d\n", ((int *)ordering_instrs->data[i])[0],
           ((int *)ordering_instrs->data[i])[1]);
    free(ordering_instrs->data[i]);
  }
  list_destroy(ordering_instrs);

  printf("\n\n");

  for (int i = 0; i < updates->len; i++) {
    List *update_line = (List *)updates->data[i];
    for (int j = 0; j < update_line->len; j++) {
      if (j != 0) {
        printf(",");
      }
      printf("%d", *((int **)update_line->data)[j]);
      free(update_line->data[j]);
    }
    list_destroy(update_line);
    printf("\n");
  }
  list_destroy(updates);
}

void part2() {}

int main() {
  part1();
  part2();
  exit(0);
}