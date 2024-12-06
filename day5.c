#include "aoc_input.h"
#include "list.h"
#include "strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1() {
  // Read in file
  List *file_lines = get_lines("inputs/day5.txt");

  // Create a list for the ordering rules and a list for the updates
  List *ordering_rules = list_create();
  List *updates = list_create();

  // Iterate over the lines of the file to add to the rules. Free the file lines
  // as we go
  bool on_updates = false;
  for (int i = 0; i < file_lines->len; i++) {
    // If the line is blank, it's the separator between the rules and updates.
    if (!on_updates && strcmp((char *)file_lines->data[i], "") == 0) {
      on_updates = true;
      free(file_lines->data[i]);
      continue;
    }

    if (!on_updates) {
      // Split the rule on the pipe character, then create a length 2 array for
      // the numbers
      List *instr = str_split((char *)file_lines->data[i], "|");
      int *nums = (int *)malloc(sizeof(int) * 2);
      nums[0] = atoi(instr->data[0]);
      nums[1] = atoi(instr->data[1]);

      // Add the numbers to the list of rules and free the memory for the split
      // string
      list_append(ordering_rules, nums);
      free(instr->data[0]);
      free(instr->data[1]);
      list_destroy(instr);
    } else {
      // Split the update list on the comma, then add as integers to a list.
      // Free the split string as we go
      List *instr = str_split((char *)file_lines->data[i], ",");
      List *nums = list_create();
      for (int j = 0; j < instr->len; j++) {
        int *num_ptr = (int *)malloc(sizeof(int));
        *num_ptr = atoi(instr->data[j]);
        list_append(nums, num_ptr);
        free(instr->data[j]);
      }
      list_append(updates, nums);
      list_destroy(instr);
    }

    // Free the line of the file
    free(file_lines->data[i]);
  }

  // Free up the list of file lines
  list_destroy(file_lines);

  //////////////////////// Part 1 ////////////////////////
  int sum = 0;

  // Iterate over the update lists to check which are ok. Then, remove the ones
  // that are OK after totaling.
  for (int i = 0; i < updates->len; i++) {
    bool has_error = false;
    List *update_line = (List *)updates->data[i];

    // Iterate over each pair of values in the line. Check the ordering of each
    // pair until an error is found or all are checked
    for (int j = 0; j < update_line->len; j++) {
      for (int k = j + 1; k < update_line->len; k++) {
        // Check each pair against each rule to see if a rule is violated
        for (int n = 0; n < ordering_rules->len; n++) {
          if (((int *)ordering_rules->data[n])[0] ==
                  *((int *)update_line->data[k]) &&
              ((int *)ordering_rules->data[n])[1] ==
                  *((int *)update_line->data[j])) {
            has_error = true;
            break;
          }
        } // end loop over rules

        if (has_error) {
          break;
        }
      } // end loop over second number in pair

      if (has_error) {
        break;
      }
    } // end loop over first number in pair

    // If there was no error, add the middle number to the sum, then delete the
    // update line.
    if (!has_error) {
      sum += *((int *)update_line->data[(update_line->len - 1) / 2]);
      for (int j = 0; j < update_line->len; j++) {
        free(update_line->data[j]);
      }
      list_destroy((List *)list_remove(updates, i));

      // decrement i to avoid skipping a line
      i--;
    }
  }

  printf("Sum: %d\n", sum);

  //////////////////////// Cleanup ////////////////////////
  for (int i = 0; i < updates->len; i++) {
    List *update_line = (List *)updates->data[i];
    for (int j = 0; j < update_line->len; j++) {
      free(update_line->data[j]);
    }
    list_destroy(update_line);
  }
  list_destroy(updates);

  for (int i = 0; i < ordering_rules->len; i++) {
    free(ordering_rules->data[i]);
  }
  list_destroy(ordering_rules);
}

void part2() {}

int main() {
  part1();
  part2();
  exit(0);
}