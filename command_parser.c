#include "command_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
void write_in_history_file(char * str);
char* lookup_variable(char* key);
int check_char(char* str, char a) {
	for (int i = 0; i < strlen(str); ++i) {
		if (str[i] == a)
			return 1;
	}
	return 0;
}
char *
str_replace(char *str, char *sub, char *replace) {
	char *pos = (char *) str;
	int size;
	if(replace==NULL) size=(strlen(str) - (strlen(sub)) ) + 1;
	else  size = (strlen(str) - (strlen(sub)) + strlen(replace)) + 1;

	char *result = (char *) malloc(size);
	if (NULL == result)
		return NULL;
	memset(result, '\0', size);
	char *current;
	while ((current = strstr(pos, sub))) {
		int len = current - pos;
		strncat(result, pos, len);
		if(replace==NULL)strncat(result, replace, 0);
		else strncat(result, replace, strlen(replace));
		pos = current + strlen(sub);
	}

	if (pos != (str + strlen(str))) {
		strncat(result, pos, (str - pos));
	}

	return result;
}
char* check(char* command) {
	for (int i = 0; i < strlen(command); ++i) {

		if (command[i] == '$') {
			char *a = malloc(sizeof(char));
			char *b = malloc(sizeof(char));
			b[0]=command[i];
			i++;
			int j = 0;
			if (isalpha(command[i]) || command[i] == '_')
				while (isalpha(command[i]) || isdigit(command[i])
						|| command[i] == '_') {
					a[j++] = command[i];
					b[j] = command[i++];
				}
			a[j] = '\0';
			command = str_replace(command, b, lookup_variable(a));
			i = 0;

		}
		if (command[i] == '~') {
			command = str_replace(command, "~", lookup_variable("HOME"));
		}
	}

	return command;

}
int count_char(char *str, char a) {
	size_t max = strlen(str);
	int count = 0;
	for (int i = 0; i < max; ++i) {
		if (str[i] == a)
			count++;
	}
	return count;
}

char** str_split(char* command, char* spiliter) {

	char ** res = NULL;
	char * p = strtok(command, spiliter);
	int n_spaces = 0;

	/* split string and append tokens to 'res' */

	while (p) {
		res = realloc(res, sizeof(char*) * ++n_spaces);

		if (res == NULL)
			exit(-1); /* memory allocation failed */

		res[n_spaces - 1] = p;

		p = strtok(NULL, spiliter);
	}

	/* realloc one extra element for the last NULL s*/

	res = realloc(res, sizeof(char*) * (n_spaces + 1));
	res[n_spaces] = 0;

	return res;
}
char *trimwhitespace(char *str) {
	char *end;

	// Trim leading space
	while (isspace((unsigned char )*str))
		str++;

	if (*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char )*end))
		end--;

	// Write new null terminator
	*(end + 1) = 0;

	return str;
}

int parse(char *line, char **argv) {

	write_in_history_file(line);
	write_in_history_file("\n");
	line = check(line);
	line = trimwhitespace(line);
	if (check_char(line, '&'))
		line = str_replace(line, "&", "\t&");
	while (*line != '\0') { /* if not the end of line ....... */
		while (*line == ' ' || *line == '\t' || *line == '\n')
			*line++ = '\0'; /* replace white spaces with 0    */
		//int i=0;

		*argv++ = line; /* save the argument position     */
//		if (strcmp(argv[0], "echo")) {
//					if(i==1)return 0;
//					else i++;
//				}
		while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
			line++; /* skip the argument until ...    */
	}
	if (*(line - 1) == '&') {
			argv--;
			*argv = '\0'; /* mark the end of argument list  */
			return 1;
		}

		else{
			*argv = '\0'; /* mark the end of argument list  */
			return 0;
		}
}
