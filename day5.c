#include "aoc_input.h"
#include "list.h"
#include "map.h"
#include "strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_rules(const void *a, const void *b) {
  int *arr_a = (int *)a;
  int *arr_b = (int *)b;

  if (arr_a[0] != arr_b[0]) {
    return arr_a[0] - arr_b[0];
  } else {
    return arr_a[1] - arr_b[1];
  }
}

int compare_nums(const void *a, const void *b) {
  int *num_a = (int *)a;
  int *num_b = (int *)b;
  return *num_a - *num_b;
}

void parts_1_and_2() {
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

  //////////////////////// Part 2 ////////////////////////
  sum = 0;

  // Iterate over all updates
  for (int u = 0; u < updates->len; u++) {
    List *update = (List *)updates->data[u];
    // Rules represent edges in a directed graph of numbers in the update. The
    // map stores which edges are in the graph
    Map *rule_map = map_create(compare_rules);

    // Add edges for all rules where both numbers are in the update
    for (int r = 0; r < ordering_rules->len; r++) {
      int first = ((int *)ordering_rules->data[r])[0];
      int second = ((int *)ordering_rules->data[r])[1];

      // Check first number in rule
      bool first_in_update = false;
      for (int i = 0; i < update->len; i++) {
        if (*((int *)update->data[i]) == first) {
          first_in_update = true;
          break;
        }
      }
      if (!first_in_update) {
        continue;
      }

      // Check second number in rule
      bool second_in_update = false;
      for (int i = 0; i < update->len; i++) {
        if (*((int *)update->data[i]) == second) {
          second_in_update = true;
          break;
        }
      }
      if (!second_in_update) {
        continue;
      }

      // Both are in, so add to the map.
      int *key = (int *)malloc(sizeof(int) * 2);
      key[0] = first;
      key[1] = second;
      bool *val = (bool *)malloc(sizeof(bool));
      *val = true;
      map_insert(rule_map, key, val);
    } // end loop over rules

    // Use Warshall's Algorithm to compute the transitive closure of the graph
    // For more info: https://cs.winona.edu/lin/cs440/ch08-2.pdf
    for (int k = 0; k < update->len; k++) {
      for (int i = 0; i < update->len; i++) {
        for (int j = 0; j < update->len; j++) {
          // If the map contains a boolean value for the edge, use that.
          // Otherwise, treat it as false.
          int rule_ij[2] = {*((int *)update->data[i]),
                            *((int *)update->data[j])};
          int rule_ik[2] = {*((int *)update->data[i]),
                            *((int *)update->data[k])};
          int rule_kj[2] = {*((int *)update->data[k]),
                            *((int *)update->data[j])};
          bool *ij_bool_ptr;
          bool ik_bool;
          bool kj_bool;

          if (map_contains(rule_map, rule_ik)) {
            ik_bool = *((bool *)map_get(rule_map, rule_ik));
          } else {
            ik_bool = false;
          }

          if (map_contains(rule_map, rule_kj)) {
            kj_bool = *((bool *)map_get(rule_map, rule_kj));
          } else {
            kj_bool = false;
          }

          if (map_contains(rule_map, rule_ij)) {
            ij_bool_ptr = (bool *)map_get(rule_map, rule_ij);
            *ij_bool_ptr = *ij_bool_ptr || (ik_bool && kj_bool);
            // Not need to do an insert/update, this updates the bool directly
          } else {
            ij_bool_ptr = (bool *)malloc(sizeof(bool));
            *ij_bool_ptr = ik_bool && kj_bool;
            int *key = (int *)malloc(sizeof(int) * 2);
            key[0] = rule_ij[0];
            key[1] = rule_ij[1];
            map_insert(rule_map, key, ij_bool_ptr);
          }
        }
      }
    } // end Warshall Algorithm loop

    Map *number_count = map_create(compare_nums);

    // Now create a mapping for each number in the closure of the rules (or
    // equivalently, each number in the update list). Subtract one from its
    // value for being the first number of a rule, and add one for being the
    // last number of a rule.
    for (int i = 0; i < rule_map->list->len; i++) {
      KeyVal *rule_keyval = (KeyVal *)rule_map->list->data[i];
      int *rule_key = (int *)rule_keyval->key;
      bool *rule_val = (bool *)rule_keyval->val;

      if (*rule_val) {
        if (map_contains(number_count, rule_key + 0)) {
          int *val = map_get(number_count, rule_key + 0);
          *val = *val - 1;
        } else {
          int *key = (int *)malloc(sizeof(int));
          int *val = (int *)malloc(sizeof(int));
          *key = rule_key[0];
          *val = -1;
          map_insert(number_count, key, val);
        }

        if (map_contains(number_count, rule_key + 1)) {
          int *val = map_get(number_count, rule_key + 1);
          *val = *val + 1;
        } else {
          int *key = (int *)malloc(sizeof(int));
          int *val = (int *)malloc(sizeof(int));
          *key = rule_key[1];
          *val = 1;
          map_insert(number_count, key, val);
        }
      }
    }

    // Now iterate over the number_count map. The number with a value of 0
    // should be added to the sum.
    for (int i = 0; i < number_count->list->len; i++) {
      KeyVal *kv = (KeyVal *)number_count->list->data[i];
      int val = *((int *)kv->val);
      int key = *((int *)kv->key);
      if (val == 0) {
        sum += key;
      }
    }

    // Destroy the rule map and number_count map
    map_destroy(rule_map);
    map_destroy(number_count);
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

int main() {
  parts_1_and_2();
  exit(EXIT_SUCCESS);
}