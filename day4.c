#include "aoc_input.h"
#include "list.h"
#include "strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1() {
  List *file_lines = get_lines("inputs/day4.txt");

  int count = 0;

  // Check forward + backward horizontal
  for (int i = 0; i < file_lines->len; i++) {
    for (int j = 0; j < strlen(file_lines->data[i]) - 3; j++) {
      if (str_starts_with(((char *)file_lines->data[i]) + j, "XMAS")) {
        count++;
        // Add 2 to j (plus one more from j++) to skip the rest of XMAS (except
        // the last letter in case of "XMASAMX")
        j += 2;
      } else if (str_starts_with(((char *)file_lines->data[i]) + j, "SAMX")) {
        count++;
        j += 2;
      }
    }
  }

  // Check forward + backward vertical
  for (int i = 0; i < strlen(file_lines->data[0]); i++) {
    for (int j = 0; j < file_lines->len - 3; j++) {
      char word[5];
      word[0] = ((char *)file_lines->data[j])[i];
      word[1] = ((char *)file_lines->data[j + 1])[i];
      word[2] = ((char *)file_lines->data[j + 2])[i];
      word[3] = ((char *)file_lines->data[j + 3])[i];
      word[4] = '\0';
      if (strcmp(word, "XMAS") == 0) {
        count++;
        j += 2;
      } else if (strcmp(word, "SAMX") == 0) {
        count++;
        j += 2;
      }
    }
  }

  // Check UL/BR and reverse diagonals
  for (int i = 0; i < file_lines->len - 3; i++) {
    for (int j = 0; j < strlen(file_lines->data[i]) - 3; j++) {
      char word[5];
      word[0] = ((char *)file_lines->data[i])[j];
      word[1] = ((char *)file_lines->data[i + 1])[j + 1];
      word[2] = ((char *)file_lines->data[i + 2])[j + 2];
      word[3] = ((char *)file_lines->data[i + 3])[j + 3];
      word[4] = '\0';
      if (strcmp(word, "XMAS") == 0) {
        count++;
        // No skipping, that's a bit too complicated to bother with on diagonals
      } else if (strcmp(word, "SAMX") == 0) {
        count++;
        // No skip
      }
    }
  }

  // Check UR/BL and reverse diagonals
  for (int i = file_lines->len - 1; i >= 3; i--) {
    for (int j = 0; j < strlen(file_lines->data[i]) - 3; j++) {
      char word[5];
      word[0] = ((char *)file_lines->data[i])[j];
      word[1] = ((char *)file_lines->data[i - 1])[j + 1];
      word[2] = ((char *)file_lines->data[i - 2])[j + 2];
      word[3] = ((char *)file_lines->data[i - 3])[j + 3];
      word[4] = '\0';
      if (strcmp(word, "XMAS") == 0) {
        count++;
        // No skipping, that's a bit too complicated to bother with on diagonals
      } else if (strcmp(word, "SAMX") == 0) {
        count++;
        // No skip
      }
    }
  }

  printf("Count: %d\n", count);

  for (int i = 0; i < file_lines->len; i++) {
    free(file_lines->data[i]);
  }
  list_destroy(file_lines);
}

void part2() {
  List *file_lines = get_lines("inputs/day4.txt");

  int count = 0;

  for (int i = 1; i < file_lines->len - 1; i++) {
    for (int j = 1; j < strlen(file_lines->data[i]) - 1; j++) {
      if (((char *)file_lines->data[i])[j] == 'A') {
        if ((((char *)file_lines->data[i - 1])[j - 1] == 'M' &&
             ((char *)file_lines->data[i + 1])[j + 1] == 'S') ||
            (((char *)file_lines->data[i - 1])[j - 1] == 'S' &&
             ((char *)file_lines->data[i + 1])[j + 1] == 'M')) {
          if ((((char *)file_lines->data[i - 1])[j + 1] == 'M' &&
               ((char *)file_lines->data[i + 1])[j - 1] == 'S') ||
              (((char *)file_lines->data[i - 1])[j + 1] == 'S' &&
               ((char *)file_lines->data[i + 1])[j - 1] == 'M')) {
                count++;
          }
        }
      }
    }
  }

  printf("Count: %d\n", count);

  for (int i = 0; i < file_lines->len; i++) {
    free(file_lines->data[i]);
  }
  list_destroy(file_lines);
}

int main() {
  part1();
  part2();
  exit(0);
}