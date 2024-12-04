#include "list.h"
#include "strings.h"
#include "aoc_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

List * get_lines(char * file_name) {
	FILE * fp = fopen(file_name, "r");
	if (!fp) {
		return NULL;
	}
	
	List * lines = list_create();
	int ch = fgetc(fp);
	char * current_line = str_create();
	while (ch != EOF) {
		if (ch != '\n') {
			current_line = str_append_char(current_line, (char) ch);
		} else {
			list_append(lines, (void *) current_line);
			current_line = str_create();
		}
		ch = fgetc(fp);
	}
	// We don't really have a case where we want to append an empty
	// line if the file ends with \n. So, we won't append if the last
	// line is empty.
	if (strlen(current_line) != 0) {
		list_append(lines, current_line);
	} else {
		free(current_line);
	}

	fclose(fp);
	
	return lines;
}
