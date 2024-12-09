#include "aoc_input.h"
#include "hashmap.h"
#include "list.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hashmap HashMap;

typedef struct {
  char frequency;
  List *instances;
} AntennaList;

uint64_t antenna_list_hash(const void *ant, uint64_t seed0, uint64_t seed1) {
  const AntennaList *a = ant;
  return hashmap_sip(&(a->frequency), sizeof(char), seed0, seed1);
}

int antenna_list_compare(const void *a, const void *b, void *other) {
  const AntennaList *ant_a = a;
  const AntennaList *ant_b = b;
  return ant_a->frequency - ant_b->frequency;
}

void antenna_list_destroy(void *item) {
  AntennaList *ant = (AntennaList *)item;
  for (int i = 0; i < ant->instances->len; i++) {
    free(ant->instances->data[i]);
  }
  list_destroy(ant->instances);
}

void part1() {
  List *file_lines = get_lines("inputs/day8.txt");

  if (file_lines == NULL) {
    fprintf(stderr, "Could not read file.\n");
    exit(EXIT_FAILURE);
  }

  int height = file_lines->len;
  int width = strlen(file_lines->data[0]);

  HashMap *antennas =
      hashmap_new(sizeof(AntennaList), 0, rand(), rand(), antenna_list_hash,
                  antenna_list_compare, antenna_list_destroy, NULL);

  for (int i = 0; i < file_lines->len; i++) {
    for (int j = 0; j < strlen(file_lines->data[i]); j++) {
      char c = ((char *)file_lines->data[i])[j];
      if (!isalnum(c)) {
        continue;
      }
      if (hashmap_get(antennas, &(AntennaList){.frequency = c}) == NULL) {
        List *instances = list_create();
        int *coords = (int *)malloc(sizeof(int) * 2);
        coords[0] = i;
        coords[1] = j;
        list_append(instances, coords);
        hashmap_set(antennas,
                    &(AntennaList){.frequency = c, .instances = instances});
      } else {
        const AntennaList *l = (const AntennaList *)hashmap_get(
            antennas, &(AntennaList){.frequency = c});
        int *coords = (int *)malloc(sizeof(int) * 2);
        coords[0] = i;
        coords[1] = j;
        list_append(l->instances, coords);
      }
    }
  }

  bool positions[height][width];
  memset(positions, false, height * width * sizeof(bool));

  size_t iter = 0;
  void *item;
  while (hashmap_iter(antennas, &iter, &item)) {
    const AntennaList *ant = (const AntennaList *)item;
    for (int i = 0; i < ant->instances->len; i++) {
      for (int j = i + 1; j < ant->instances->len; j++) {
        int vert_dist = ((int *)ant->instances->data[i])[0] -
                        ((int *)ant->instances->data[j])[0];
        int horiz_dist = ((int *)ant->instances->data[i])[1] -
                         ((int *)ant->instances->data[j])[1];

        int antinode[2] = {((int *)ant->instances->data[i])[0] + vert_dist,
                           ((int *)ant->instances->data[i])[1] + horiz_dist};
        if (antinode[0] >= 0 && antinode[0] < height && antinode[1] >= 0 &&
            antinode[1] < width) {
          positions[antinode[0]][antinode[1]] = true;
        }
        antinode[0] = ((int *)ant->instances->data[j])[0] - vert_dist;
        antinode[1] = ((int *)ant->instances->data[j])[1] - horiz_dist;
        if (antinode[0] >= 0 && antinode[0] < height && antinode[1] >= 0 &&
            antinode[1] < width) {
          positions[antinode[0]][antinode[1]] = true;
        }
      }
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

  printf("Count: %d\n", count);

  // Cleanup
  for (int i = 0; i < file_lines->len; i++) {
    free(file_lines->data[i]);
  }
  list_destroy(file_lines);
  hashmap_free(antennas);
}

void part2() {
  List *file_lines = get_lines("inputs/day8.txt");

  if (file_lines == NULL) {
    fprintf(stderr, "Could not read file.\n");
    exit(EXIT_FAILURE);
  }

  int height = file_lines->len;
  int width = strlen(file_lines->data[0]);

  HashMap *antennas =
      hashmap_new(sizeof(AntennaList), 0, rand(), rand(), antenna_list_hash,
                  antenna_list_compare, antenna_list_destroy, NULL);

  for (int i = 0; i < file_lines->len; i++) {
    for (int j = 0; j < strlen(file_lines->data[i]); j++) {
      char c = ((char *)file_lines->data[i])[j];
      if (!isalnum(c)) {
        continue;
      }
      if (hashmap_get(antennas, &(AntennaList){.frequency = c}) == NULL) {
        List *instances = list_create();
        int *coords = (int *)malloc(sizeof(int) * 2);
        coords[0] = i;
        coords[1] = j;
        list_append(instances, coords);
        hashmap_set(antennas,
                    &(AntennaList){.frequency = c, .instances = instances});
      } else {
        const AntennaList *l = (const AntennaList *)hashmap_get(
            antennas, &(AntennaList){.frequency = c});
        int *coords = (int *)malloc(sizeof(int) * 2);
        coords[0] = i;
        coords[1] = j;
        list_append(l->instances, coords);
      }
    }
  }

  bool positions[height][width];
  memset(positions, false, height * width * sizeof(bool));

  size_t iter = 0;
  void *item;
  while (hashmap_iter(antennas, &iter, &item)) {
    const AntennaList *ant = (const AntennaList *)item;
    for (int i = 0; i < ant->instances->len; i++) {
      int ant_a_pos[2] = {((int *)ant->instances->data[i])[0],
                          ((int *)ant->instances->data[i])[1]};
      // Mark self position as antinode unless there's only one of this
      // frequency
      if (ant->instances->len > 1) {
        positions[ant_a_pos[0]][ant_a_pos[1]] = true;
      }

      for (int j = i + 1; j < ant->instances->len; j++) {
        int ant_b_pos[2] = {((int *)ant->instances->data[j])[0],
                            ((int *)ant->instances->data[j])[1]};

        int vert_dist = ant_a_pos[0] - ant_b_pos[0];
        int horiz_dist = ant_a_pos[1] - ant_b_pos[1];

        int antinode[2] = {ant_a_pos[0] + vert_dist, ant_a_pos[1] + horiz_dist};
        while (antinode[0] >= 0 && antinode[0] < height && antinode[1] >= 0 &&
               antinode[1] < width) {
          positions[antinode[0]][antinode[1]] = true;
          antinode[0] += vert_dist;
          antinode[1] += horiz_dist;
        }

        antinode[0] = ant_b_pos[0] - vert_dist;
        antinode[1] = ant_b_pos[1] - horiz_dist;
        while (antinode[0] >= 0 && antinode[0] < height && antinode[1] >= 0 &&
               antinode[1] < width) {
          positions[antinode[0]][antinode[1]] = true;
          antinode[0] -= vert_dist;
          antinode[1] -= horiz_dist;
        }
      }
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

  printf("Count: %d\n", count);

  // Cleanup
  for (int i = 0; i < file_lines->len; i++) {
    free(file_lines->data[i]);
  }
  list_destroy(file_lines);
  hashmap_free(antennas);
}

int main() {
  part1();
  part2();
  exit(0);
}