#ifndef _STRINGS_H_
#define _STRINGS_H_

#include "list.h"
#include <stdbool.h>

char * str_create();

char * str_append_char(char * str, char ch);

bool str_starts_with(char * haystack, char * needle);

List * str_split(char * str, char * splitter);

#endif
