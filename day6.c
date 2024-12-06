#include "aoc_input.h"
#include "list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UP 0x1
#define DOWN 0x2
#define LEFT 0x4
#define RIGHT 0x8

void part1() {
  List *file_lines = get_lines("inputs/day6.txt");

  if (file_lines == NULL) {
    fprintf(stderr, "Could not read file.\n");
    exit(EXIT_FAILURE);
  }

  int width = strlen(file_lines->data[0]);
  int height = file_lines->len;
  int guard_pos[2] = {-1, -1};
  uint8_t dir = UP;
  uint8_t positions[height][width];
  memset(positions, 0x0, height * width);

  // Find starting position of guard
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (((char *)file_lines->data[i])[j] == '^') {
        guard_pos[0] = i;
        guard_pos[1] = j;
        break;
      }
    }

    if (guard_pos[0] != -1 && guard_pos[1] != -1) {
      break;
    }
  }

  while (guard_pos[0] >= 0 && guard_pos[0] < height && guard_pos[1] >= 0 &&
         guard_pos[1] < width) {
    // For part 1, always set direction to UP since we don't care about which
    // direction the guard is facing at a position
    positions[guard_pos[0]][guard_pos[1]] = UP;
    switch (dir) {
    case UP:
      if (guard_pos[0] != 0 &&
          ((char *)file_lines->data[guard_pos[0] - 1])[guard_pos[1]] == '#') {
        dir = RIGHT;
      } else {
        guard_pos[0]--;
      }
      break;
    case DOWN:
      if (guard_pos[0] != height - 1 &&
          ((char *)file_lines->data[guard_pos[0] + 1])[guard_pos[1]] == '#') {
        dir = LEFT;
      } else {
        guard_pos[0]++;
      }
      break;
    case LEFT:
      if (guard_pos[1] != 0 &&
          ((char *)file_lines->data[guard_pos[0]])[guard_pos[1] - 1] == '#') {
        dir = UP;
      } else {
        guard_pos[1]--;
      }
      break;
    case RIGHT:
      if (guard_pos[1] != width - 1 &&
          ((char *)file_lines->data[guard_pos[0]])[guard_pos[1] + 1] == '#') {
        dir = DOWN;
      } else {
        guard_pos[1]++;
      }
      break;
    default:
      fprintf(stderr, "Error: invalid direction.\n");
      exit(EXIT_FAILURE);
    }
  }

  int count = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (positions[i][j]) {
        count++;
      }
    }
  }
  printf("Unique positions: %d\n", count);
  list_destroy(file_lines);
}

void part2() {
  List *file_lines = get_lines("inputs/day6.txt");

  if (file_lines == NULL) {
    fprintf(stderr, "Could not read file.\n");
    exit(EXIT_FAILURE);
  }

  int width = strlen(file_lines->data[0]);
  int height = file_lines->len;
  int guard_pos[2] = {-1, -1};
  int start_pos[2];
  int blank_pos[2] = {-1, -1};

  uint8_t dir = UP;
  uint8_t positions[height][width];
  memset(positions, 0x0, height * width);

  // Find starting position of guard
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (((char *)file_lines->data[i])[j] == '^') {
        guard_pos[0] = i;
        guard_pos[1] = j;
      }
      if (blank_pos[0] == -1 && blank_pos[1] == -1 &&
          ((char *)file_lines->data[i])[j] == '.') {
        blank_pos[0] = i;
        blank_pos[1] = j;
      }
      if (blank_pos[0] != -1 && blank_pos[1] != -1 && guard_pos[0] != -1 &&
          guard_pos[1] != -1) {
        break;
      }
    }

    if (blank_pos[0] != -1 && blank_pos[1] != -1 && guard_pos[0] != -1 &&
        guard_pos[1] != -1) {
      break;
    }
  }

  start_pos[0] = guard_pos[0];
  start_pos[1] = guard_pos[1];

  int loop_count = 0;

  while (blank_pos[0] < height) {
    while (blank_pos[1] < width) {
      // If position isn't blank, skip.
      if (((char *)file_lines->data[blank_pos[0]])[blank_pos[1]] != '.') {
        blank_pos[1]++;
        continue;
      }

      ((char *)file_lines->data[blank_pos[0]])[blank_pos[1]] = '#';

      while (guard_pos[0] >= 0 && guard_pos[0] < height && guard_pos[1] >= 0 &&
             guard_pos[1] < width) {

        if (positions[guard_pos[0]][guard_pos[1]] & dir) {
          loop_count++;
          break;
        }
        positions[guard_pos[0]][guard_pos[1]] =
            positions[guard_pos[0]][guard_pos[1]] | dir;

        switch (dir) {
        case UP:
          if (guard_pos[0] != 0 &&
              ((char *)file_lines->data[guard_pos[0] - 1])[guard_pos[1]] ==
                  '#') {
            dir = RIGHT;
          } else {
            guard_pos[0]--;
          }
          break;
        case DOWN:
          if (guard_pos[0] != height - 1 &&
              ((char *)file_lines->data[guard_pos[0] + 1])[guard_pos[1]] ==
                  '#') {
            dir = LEFT;
          } else {
            guard_pos[0]++;
          }
          break;
        case LEFT:
          if (guard_pos[1] != 0 &&
              ((char *)file_lines->data[guard_pos[0]])[guard_pos[1] - 1] ==
                  '#') {
            dir = UP;
          } else {
            guard_pos[1]--;
          }
          break;
        case RIGHT:
          if (guard_pos[1] != width - 1 &&
              ((char *)file_lines->data[guard_pos[0]])[guard_pos[1] + 1] ==
                  '#') {
            dir = DOWN;
          } else {
            guard_pos[1]++;
          }
          break;
        default:
          fprintf(stderr, "Error: invalid direction.\n");
          exit(EXIT_FAILURE);
        }
      } // end guard pos loop
      ((char *)file_lines->data[blank_pos[0]])[blank_pos[1]] = ' ';
      blank_pos[1] += 1;
      guard_pos[0] = start_pos[0];
      guard_pos[1] = start_pos[1];
      // hashmap_clear(positions, false);
      memset(positions, 0x0, height * width);
      dir = UP;
    } // end blank pos width loop
    blank_pos[1] = 0;
    blank_pos[0] += 1;
  } // end blank pos height loop

  printf("Number of loops: %d\n", loop_count);
  // hashmap_free(positions);
  list_destroy(file_lines);
}

int main() {
  part1();
  part2();
  exit(0);
}