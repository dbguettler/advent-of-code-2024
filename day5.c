#include "aoc_input.h"
#include "hashmap.h"
#include "list.h"
#include "map.h"
#include "strings.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rule_element_struct {
  int key[2];
  bool val;
} RuleElement;

typedef struct number_count_element_struct {
  int key;
  int val;
} NumberCountElement;

typedef struct hashmap HashMap;

uint64_t rule_hash(const void *rule, uint64_t seed0, uint64_t seed1) {
  const RuleElement *r = rule;
  return hashmap_sip(r->key, sizeof(int) * 2, seed0, seed1);
}

uint64_t number_count_hash(const void *elem, uint64_t seed0, uint64_t seed1) {
  const NumberCountElement *nc = elem;
  return hashmap_sip(&(nc->key), sizeof(int), seed0, seed1);
}

int rule_compare(const void *a, const void *b, void *other) {
  const RuleElement *r_a = a;
  const RuleElement *r_b = b;
  const int *list_a = r_a->key;
  const int *list_b = r_b->key;
  if (list_a[0] != list_b[0]) {
    return list_a[0] - list_b[0];
  } else {
    return list_a[1] - list_b[1];
  }
}

int number_count_compare(const void *a, const void *b, void *other) {
  const NumberCountElement *nc_a = a;
  const NumberCountElement *nc_b = b;
  return nc_a->key - nc_b->key;
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
    HashMap *rule_hashmap = hashmap_new(sizeof(RuleElement), 0, rand(), rand(),
                                        rule_hash, rule_compare, NULL, NULL);

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
      hashmap_set(rule_hashmap,
                  &(RuleElement){.key = {first, second}, .val = true});
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
          bool ij_bool;
          bool ik_bool;
          bool kj_bool;

          if (hashmap_get(rule_hashmap,
                          &(RuleElement){.key = {rule_ik[0], rule_ik[1]}}) !=
              NULL) {
            ik_bool = ((RuleElement *)hashmap_get(
                           rule_hashmap,
                           &(RuleElement){.key = {rule_ik[0], rule_ik[1]}}))
                          ->val;
          } else {
            ik_bool = false;
          }

          if (hashmap_get(rule_hashmap,
                          &(RuleElement){.key = {rule_kj[0], rule_kj[1]}}) !=
              NULL) {
            kj_bool = ((RuleElement *)hashmap_get(
                           rule_hashmap,
                           &(RuleElement){.key = {rule_kj[0], rule_kj[1]}}))
                          ->val;
          } else {
            kj_bool = false;
          }

          if (hashmap_get(rule_hashmap,
                          &(RuleElement){.key = {rule_ij[0], rule_ij[1]}}) !=
              NULL) {
            ij_bool = ((RuleElement *)hashmap_get(
                           rule_hashmap,
                           &(RuleElement){.key = {rule_ij[0], rule_ij[1]}}))
                          ->val;
          } else {
            ij_bool = false;
          }

          hashmap_set(rule_hashmap,
                      &(RuleElement){.key = {rule_ij[0], rule_ij[1]},
                                     .val = ij_bool || (ik_bool && kj_bool)});
        }
      }
    } // end Warshall Algorithm loop

    HashMap *number_count_hashmap =
        hashmap_new(sizeof(NumberCountElement), 0, rand(), rand(),
                    number_count_hash, number_count_compare, NULL, NULL);

    // Now create a mapping for each number in the closure of the rules (or
    // equivalently, each number in the update list). Subtract one from its
    // value for being the first number of a rule, and add one for being the
    // last number of a rule.
    size_t iter = 0;
    void *item;
    while (hashmap_iter(rule_hashmap, &iter, &item)) {
      const RuleElement *rule_element = item;
      const int *rule_key = rule_element->key;
      const bool rule_val = rule_element->val;

      if (rule_val) {
        int count = 0;
        if (hashmap_get(number_count_hashmap,
                        &(NumberCountElement){.key = rule_key[0]}) != NULL) {
          count = ((NumberCountElement *)hashmap_get(
                           number_count_hashmap,
                           &(NumberCountElement){.key = rule_key[0]}))
                          ->val;
        }
        hashmap_set(
            number_count_hashmap,
            &(NumberCountElement){.key = rule_key[0], .val = count - 1});

        count = 0;
        if (hashmap_get(number_count_hashmap,
                        &(NumberCountElement){.key = rule_key[1]}) != NULL) {
          count = ((NumberCountElement *)hashmap_get(
                           number_count_hashmap,
                           &(NumberCountElement){.key = rule_key[1]}))
                          ->val;
        }
        hashmap_set(
            number_count_hashmap,
            &(NumberCountElement){.key = rule_key[1], .val = count + 1});
      }
    }

    // Now iterate over the number_count map. The number with a value of 0
    // should be added to the sum.
    iter = 0;
    while (hashmap_iter(number_count_hashmap, &iter, &item)) {
      const NumberCountElement *nc_element = item;
      if (nc_element->val == 0) {
        sum += nc_element->key;
      }
    }

    // Destroy the rule map and number_count map
    hashmap_free(rule_hashmap);
    hashmap_free(number_count_hashmap);
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