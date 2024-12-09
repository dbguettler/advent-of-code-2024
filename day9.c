#include "aoc_input.h"
#include "list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_struct {
  struct node_struct *prev;
  int id;
  int file_size;
  int empty_space_trailing;
  struct node_struct *next;
};

typedef struct node_struct Node;

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

  // Copy file IDs into disk
  char *line = file_lines->data[0];
  char num_str[2] = {'\0', '\0'};
  Node *first;
  Node *last;
  first = (Node *)malloc(sizeof(Node));
  first->id = 0;
  first->prev = NULL;
  first->next = NULL;
  num_str[0] = line[0];
  first->file_size = atoi(num_str);
  num_str[0] = line[1];
  first->empty_space_trailing = atoi(num_str);
  last = first;

  // Create linked list of nodes
  for (int i = 2; i < strlen(line); i += 2) {
    Node *current = (Node *)malloc(sizeof(Node));
    last->next = current;
    current->id = i / 2;
    num_str[0] = line[i];
    current->file_size = atoi(num_str);
    num_str[0] = line[i + 1];
    current->empty_space_trailing = atoi(num_str);
    current->prev = last;
    last = current;
  }

  last->next = NULL;

  // Find the earliest section of usable space
  Node *has_space = first;
  while (has_space->empty_space_trailing <= 0) {
    has_space = has_space->next;
  }

  // Copy files from end to beginning
  Node *current = last;
  int current_id = current->id;

  while (current->id != 0) {
    Node *sufficient_space = has_space;
    while (sufficient_space != NULL && sufficient_space != current &&
           sufficient_space->empty_space_trailing < current->file_size) {
      sufficient_space = sufficient_space->next;
    }

    if (sufficient_space != NULL && sufficient_space != current) {
      // We can move this file earlier. Do the following:
      // Save a pointer to the current file's predecessor.
      Node *old_predecessor = current->prev;
      // Add the current file's size and its trailing space to its predecessor.
      current->prev->empty_space_trailing +=
          current->file_size + current->empty_space_trailing;
      // Unlink the current file and link its predecessor and successor.
      current->prev->next = current->next;
      if (current->next != NULL) {
        current->next->prev = current->prev;
      }
      // Link the current file into the earlier position.
      current->prev = sufficient_space;
      current->next = sufficient_space->next;
      sufficient_space->next = current;
      current->next->prev = current;
      // Adjust the trailing space of the current file.
      current->empty_space_trailing =
          current->prev->empty_space_trailing - current->file_size;
      // Reduce the trailing space of the current file's new predecessor to 0.
      current->prev->empty_space_trailing = 0;
      // Set the current pointer to the old predecessor to enable finding the
      // next file to move.
      current = old_predecessor;
    }

    // Now, find the next file to move back. It should have an id that is one
    // less than current_id.
    while (current->id != current_id - 1) {
      current = current->prev;
    }
    current_id--;
  }

  long checksum = 0;
  int position = 0;
  current = first;
  while (current != NULL) {
    for (int i = 0; i < current->file_size; i++) {
      checksum += current->id * (position + i);
    }

    position += current->file_size + current->empty_space_trailing;
    Node* to_free = current;
    current = current->next;
    free(to_free);
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