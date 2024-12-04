#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "strings.h"
#include "list.h"


char * str_create() {
	char * str = (char *) malloc(sizeof(char));
	str[0] = '\0';
	return str;
}

/**
 * Append the given character to the end of the string.
 * 
 * @param str The string to append the character to
 * @param ch The character to append to the string
 */
char * str_append_char(char * str, char ch) {
	str = (char *) realloc(str, sizeof(char) * (strlen(str) + 2));
	str[strlen(str) + 1] = '\0';
	str[strlen(str)] = ch;
	return str;
}

bool str_starts_with(char * haystack, char * needle) {
	if (strlen(needle) > strlen(haystack)) {
		return false;
	}

	for (int i = 0; i < strlen(needle); i++) {
		if (haystack[i] != needle[i]) {
			return false;
		}
	}

	return true;
}

List * str_split(char * str, char * splitter) {
  List * l = list_create();

  char * token = strtok(str, splitter);

  while (token != NULL) {
    char * list_item = (char *) malloc(sizeof(char) * strlen(token) + 1);
    strcpy(list_item, token);
    list_append(l, list_item);
    token = strtok(NULL, splitter);
  }

  return l;
}