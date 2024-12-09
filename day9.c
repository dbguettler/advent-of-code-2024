#include "aoc_input.h"
#include "list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1() {
  List *file_lines = get_lines("inputs/day9.txt");

  if (file_lines == NULL) {
    fprintf(stderr, "Could not read file.\n");
    exit(EXIT_FAILURE);
  }

  if (file_lines->len != 1) {
    fprintf(stderr,
            "Error: file should only have one line of data for day 9.\n");
    exit(EXIT_FAILURE);
  }

  int total_blocks = 0;
  char *line = file_lines->data[0];
  char num_str[2] = {'\0', '\0'};
  for (int i = 0; i < strlen(line); i++) {
    num_str[0] = line[i];
    total_blocks += atoi(num_str);
  }

  printf("Disk size: %d blocks (%lu bytes)\n", total_blocks,
         total_blocks * sizeof(int));

  // Copy file IDs into disk
  int disk[total_blocks];
  int last_block = 0;
  int current_block = 0;
  int first_empty = 0;
  for (int i = 0; i < strlen(line); i++) {
    int file_id = i / 2;
    num_str[0] = line[i];
    int space = atoi(num_str);
    if (i == 1) {
      first_empty = current_block;
    }
    for (int j = 0; j < space; j++) {
      if (i % 2 == 0) {
        // file space
        disk[current_block] = file_id;
        last_block = current_block;
      } else {
        // free space
        if (j == 0) {
          disk[current_block] = -1 * space;
        } else {
          disk[current_block] = -1;
        }
      }
      current_block++;
    }
  }

  // Copy file blocks from end to beginning
  while (first_empty <= last_block) {
    disk[first_empty] = disk[last_block];
    disk[last_block] = -1;
    do {
      first_empty++;
    } while (disk[first_empty] >= 0);

    do {
      last_block--;
    } while (disk[last_block] < 0);
  }

  long checksum = 0;
  for (int i = 0; i < first_empty; i++) {
    checksum += i * disk[i];
  }

  printf("Part 1 Checksum: %ld\n", checksum);

  // Cleanup
  for (int i = 0; i < file_lines->len; i++) {
    free(file_lines->data[i]);
  }
  list_destroy(file_lines);
}

void part2() {
  List *file_lines = get_lines("inputs/day9.txt");

  if (file_lines == NULL) {
    fprintf(stderr, "Could not read file.\n");
    exit(EXIT_FAILURE);
  }

  if (file_lines->len != 1) {
    fprintf(stderr,
            "Error: file should only have one line of data for day 9.\n");
    exit(EXIT_FAILURE);
  }

  int total_blocks = 0;
  char *line = file_lines->data[0];
  char num_str[2] = {'\0', '\0'};
  for (int i = 0; i < strlen(line); i++) {
    num_str[0] = line[i];
    total_blocks += atoi(num_str);
  }

  // Copy file IDs into disk
  int disk[total_blocks];
  int last_file_end_idx = 0;
  int current_block = 0;
  int first_empty = 0;
  for (int i = 0; i < strlen(line); i++) {
    int file_id = i / 2;
    num_str[0] = line[i];
    int space = atoi(num_str);
    if (i == 1) {
      first_empty = current_block;
    }
    for (int j = 0; j < space; j++) {
      if (i % 2 == 0) {
        // file space
        disk[current_block] = file_id;
        last_file_end_idx = current_block;

      } else {
        // free space
        if (j == 0) {
          disk[current_block] = -1 * space;
        } else {
          disk[current_block] = -1;
        }
      }
      current_block++;
    }
  }

  // Copy files from end to beginning
  for (int i = disk[last_file_end_idx]; i > 0; i--) {
    // Get the end position of the file with id i
    int last_file_idx = last_file_end_idx;
    while (last_file_idx >= 0 && disk[last_file_idx] == i) {
      last_file_idx--;
    }
    last_file_idx++;

    int file_size = (last_file_end_idx - last_file_idx + 1);

    // Find the first contiguous space that can accomodate the file located
    // before the file
    int empty_idx = first_empty;
    while (empty_idx < last_file_idx) {
      if (disk[empty_idx] * -1 >= file_size) {
        // Space is big enough
        break;
      }

      // Space is not big enough. Find the next empty block
      do {
        empty_idx++;
      } while (disk[empty_idx] >= 0);
    }

    // empty_idx is either the index of a block that can accomodate the file, or
    // is equal to the starting index of the file.
    if (empty_idx < last_file_idx) {
      int empty_size = disk[empty_idx] * -1;
      // Copy file.
      for (int j = 0; j < file_size; j++) {
        disk[empty_idx + j] = disk[last_file_end_idx];
        disk[last_file_end_idx] = -1;
        last_file_end_idx--;
      }

      // Update first_empty if empty_idx == first_empty
      if (first_empty == empty_idx) {
        first_empty += file_size;
      }

      // If the next space after the copied location is empty, update the empty
      // space block size
      if (disk[empty_idx + file_size] < 0) {
        disk[empty_idx + file_size] = (empty_size - file_size) * -1;
      }
    }

    // Adjust the end index if necessary
    while (disk[last_file_end_idx] != i - 1) {
      last_file_end_idx--;
    }
  }

  long checksum = 0;
  for (int i = 0; i < total_blocks; i++) {
    if (disk[i] >= 0) {
      checksum += i * disk[i];
    }
  }

  printf("Part 2 Checksum: %ld\n", checksum);

  // Cleanup
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