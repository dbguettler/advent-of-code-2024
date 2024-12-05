#include "aoc_input.h"
#include "list.h"
#include "strings.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum state {
  AWAIT_M_OR_D,
  FOUND_M,
  FOUND_MU,
  FOUND_MUL,
  READ_NUM_ONE,
  READ_NUM_TWO,
  FOUND_D_ENABLED,
  FOUND_D_DISABLED,
  FOUND_DO_ENABLED,
  FOUND_DO_DISABLED,
  FOUND_DO_OPAR,
  FOUND_DON,
  FOUND_DON_QOT,
  FOUND_DONT,
  FOUND_DONT_OPAR,
  AWAIT_D
};

void part2() {
  List *file_lines = get_lines("inputs/day3.txt");

  int value = 0;
  int first = 0;
  int second = 0;
  char *num_str = str_create();
  enum state state = AWAIT_M_OR_D;

  for (int i = 0; i < file_lines->len; i++) {
    char *line = file_lines->data[i];

    if (state == FOUND_D_DISABLED || state == FOUND_DO_DISABLED ||
        state == FOUND_DO_OPAR || state == AWAIT_D) {
          state = AWAIT_D;
    } else {
      state = AWAIT_M_OR_D;
    }

    for (int j = 0; j < strlen(line); j++) {
      switch (state) {
      case AWAIT_M_OR_D:
        if (line[j] == 'm') {
          state = FOUND_M;
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
        }
        break;
      case FOUND_M:
        if (line[j] == 'u') {
          state = FOUND_MU;
        } else if (line[j] == 'm') {
          state = FOUND_M;
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
        } else {
          state = AWAIT_M_OR_D;
        }
        break;
      case FOUND_MU:
        if (line[j] == 'l') {
          state = FOUND_MUL;
        } else if (line[j] == 'm') {
          state = FOUND_M;
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
        } else {
          state = AWAIT_M_OR_D;
        }
        break;
      case FOUND_MUL:
        if (line[j] == '(') {
          state = READ_NUM_ONE;
        } else if (line[j] == 'm') {
          state = FOUND_M;
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
        } else {
          state = AWAIT_M_OR_D;
        }
        break;
      case READ_NUM_ONE:
        if (isdigit(line[j])) {
          num_str = str_append_char(num_str, line[j]);
        } else if (line[j] == ',') {
          state = READ_NUM_TWO;
          first = atoi(num_str);
          free(num_str);
          num_str = str_create();
        } else if (line[j] == 'm') {
          state = FOUND_M;
          free(num_str);
          num_str = str_create();
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
          free(num_str);
          num_str = str_create();
        } else {
          state = AWAIT_M_OR_D;
          free(num_str);
          num_str = str_create();
        }
        break;
      case READ_NUM_TWO:
        if (isdigit(line[j])) {
          num_str = str_append_char(num_str, line[j]);
        } else if (line[j] == ')') {
          state = AWAIT_M_OR_D;
          second = atoi(num_str);
          free(num_str);
          num_str = str_create();
          value += first * second;
        } else if (line[j] == 'm') {
          state = FOUND_M;
          free(num_str);
          num_str = str_create();
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
          free(num_str);
          num_str = str_create();
        } else {
          state = AWAIT_M_OR_D;
          free(num_str);
          num_str = str_create();
        }
        break;
      case FOUND_D_ENABLED:
        if (line[j] == 'o') {
          state = FOUND_DO_ENABLED;
        } else if (line[j] == 'm') {
          state = FOUND_M;
        } else {
          state = AWAIT_M_OR_D;
        }
        break;
      case FOUND_DO_ENABLED:
        if (line[j] == 'n') {
          state = FOUND_DON;
        } else if (line[j] == 'm') {
          state = FOUND_M;
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
        } else {
          state = AWAIT_M_OR_D;
        }
        break;
      case FOUND_D_DISABLED:
        if (line[j] == 'o') {
          state = FOUND_DO_DISABLED;
        } else if (line[j] == 'd') {
          state = FOUND_D_DISABLED;
        } else {
          state = AWAIT_D;
        }
        break;
      case FOUND_DO_DISABLED:
        if (line[j] == '(') {
          state = FOUND_DO_OPAR;
        } else if (line[j] == 'd') {
          state = FOUND_D_DISABLED;
        } else {
          state = AWAIT_D;
        }
        break;
      case FOUND_DO_OPAR:
        if (line[j] == ')') {
          state = AWAIT_M_OR_D;
        } else if (line[j] == 'd') {
          state = FOUND_D_DISABLED;
        } else {
          state = AWAIT_D;
        }
        break;
      case FOUND_DON:
        if (line[j] == '\'') {
          state = FOUND_DON_QOT;
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
        } else if (line[j] == 'm') {
          state = FOUND_M;
        } else {
          state = AWAIT_M_OR_D;
        }
        break;
      case FOUND_DON_QOT:
        if (line[j] == 't') {
          state = FOUND_DONT;
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
        } else if (line[j] == 'm') {
          state = FOUND_M;
        } else {
          state = AWAIT_M_OR_D;
        }
        break;
      case FOUND_DONT:
        if (line[j] == '(') {
          state = FOUND_DONT_OPAR;
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
        } else if (line[j] == 'm') {
          state = FOUND_M;
        } else {
          state = AWAIT_M_OR_D;
        }
        break;
      case FOUND_DONT_OPAR:
        if (line[j] == ')') {
          state = AWAIT_D;
        } else if (line[j] == 'd') {
          state = FOUND_D_ENABLED;
        } else if (line[j] == 'm') {
          state = FOUND_M;
        } else {
          state = AWAIT_M_OR_D;
        }
        break;
      case AWAIT_D:
        if (line[j] == 'd') {
          state = FOUND_D_DISABLED;
        }
        break;
      default:
        printf("Invalid State!\n");
        exit(1);
      }
    }
  }

  free(num_str);
  printf("Value: %d\n", value);

  for (int i = 0; i < file_lines->len; i++) {
    free(file_lines->data[i]);
  }

  list_destroy(file_lines);
}

void part1() {
  List *file_lines = get_lines("inputs/day3.txt");

  int value = 0;
  int state = 0;
  int first = 0;
  int second = 0;
  char *num_str = str_create();

  for (int i = 0; i < file_lines->len; i++) {
    state = 0;
    char *line = file_lines->data[i];
    for (int j = 0; j < strlen(line); j++) {
      /* States are as follows:
       * 0: Not yet found 'm'
       * 1: Found 'm'; awaiting 'u'
       * 2: Found 'u'; awaiting 'l'
       * 3: Found 'l'; awaiting '('
       * 4: Found '('; reading first number
       * 5: Found ','; reading second number
       */
      if (state == 0) {
        if (line[j] == 'm') {
          state = 1;
        }
      } else if (state == 1) {
        if (line[j] == 'u') {
          state = 2;
        } else if (line[j] == 'm') {
          state = 1;
        } else {
          state = 0;
        }
      } else if (state == 2) {
        if (line[j] == 'l') {
          state = 3;
        } else if (line[j] == 'm') {
          state = 1;
        } else {
          state = 0;
        }
      } else if (state == 3) {
        if (line[j] == '(') {
          state = 4;
        } else if (line[j] == 'm') {
          state = 1;
        } else {
          state = 0;
        }
      } else if (state == 4) {
        if (isdigit(line[j])) {
          num_str = str_append_char(num_str, line[j]);
        } else if (line[j] == ',') {
          state = 5;
          first = atoi(num_str);
          free(num_str);
          num_str = str_create();
        } else if (line[j] == 'm') {
          state = 1;
          free(num_str);
          num_str = str_create();
        } else {
          state = 0;
          free(num_str);
          num_str = str_create();
        }
      } else if (state == 5) {
        if (isdigit(line[j])) {
          num_str = str_append_char(num_str, line[j]);
        } else if (line[j] == ')') {
          state = 0;
          second = atoi(num_str);
          free(num_str);
          num_str = str_create();
          value += first * second;
        } else if (line[j] == 'm') {
          state = 1;
          free(num_str);
          num_str = str_create();
        } else {
          state = 0;
          free(num_str);
          num_str = str_create();
        }
      } else {
        printf("Invalid State!\n");
        exit(1);
      }
    }
  }

  free(num_str);
  printf("Value: %d\n", value);

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