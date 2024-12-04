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
  List * file_lines = get_lines("inputs/day2.txt");

  int safe_reports = 0;

  for (int i = 0; i < file_lines->len; i++) {
    List * split_line = str_split(file_lines->data[i], " ");
    int levels[split_line->len];
    int count = split_line->len;
    
    for (int j = 0; j < count; j++) {
      levels[j] = atoi(split_line->data[j]);
      free(split_line->data[j]);
    }

    safe_reports++;
    bool increasing = true;
    bool decreasing = true;
    bool differ_ok = true;

    for (int j = 1; j < count; j++) {
      increasing = increasing && levels[j - 1] < levels[j];
      decreasing = decreasing && levels[j - 1] > levels[j];
      differ_ok = differ_ok && levels[j - 1] - levels[j] > -4 && levels[j - 1] - levels[j] < 4;

      if (!differ_ok || (!decreasing && !increasing)) {
        safe_reports--;
        break;
      }
    }

    list_destroy(split_line);
    free(file_lines->data[i]);
  }

  list_destroy(file_lines);

  printf("Safe reports: %d\n", safe_reports);
}

void part2() {
  List * file_lines = get_lines("inputs/day2.txt");
  int safe_reports = 0;

  for (int i = 0; i < file_lines->len; i++) {
    List * split_line = str_split(file_lines->data[i], " ");
    int levels[split_line->len];
    int count = split_line->len;
    
    for (int j = 0; j < count; j++) {
      levels[j] = atoi(split_line->data[j]);
      free(split_line->data[j]);
    }

    safe_reports++;
    bool increasing = true;
    bool decreasing = true;
    bool differ_ok = true;

    for (int j = 1; j < count; j++) {
      increasing = increasing && levels[j - 1] < levels[j];
      decreasing = decreasing && levels[j - 1] > levels[j];
      differ_ok = differ_ok && levels[j - 1] - levels[j] > -4 && levels[j - 1] - levels[j] < 4;

      if (!differ_ok || (!decreasing && !increasing)) {
        safe_reports--;
        break;
      }
    }

    if (!differ_ok || (!decreasing && !increasing)) {
      // Levels are not ok. Check if eliminating a value fixes it.
      for (int skip = 0; skip < count; skip++) {
        increasing = true;
        decreasing = true;
        differ_ok = true;
        for (int j = 1; j < count; j++) {
          if (skip == j - 1) {
            if (j == 1) {
              continue;
            } else {
              increasing = increasing && levels[j - 2] < levels[j];
              decreasing = decreasing && levels[j - 2] > levels[j];
              differ_ok = differ_ok && levels[j - 2] - levels[j] > -4 && levels[j - 2] - levels[j] < 4;
            }
          } else if (skip == j) {
            continue;
          } else {
            increasing = increasing && levels[j - 1] < levels[j];
            decreasing = decreasing && levels[j - 1] > levels[j];
            differ_ok = differ_ok && levels[j - 1] - levels[j] > -4 && levels[j - 1] - levels[j] < 4;
          }

          if (!differ_ok || (!decreasing && !increasing)) {
            break;
          }
        }

        if (differ_ok && (decreasing || increasing)) {
          safe_reports++;
          break;
        }
      }
    }

    list_destroy(split_line);
    free(file_lines->data[i]);
  }

  list_destroy(file_lines);

  printf("Safe reports: %d\n", safe_reports);
}

int main() {
  part1();
  part2();
  exit(0);
}