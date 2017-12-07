#include "file_processing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* 
 history file section

 */
FILE* history_file, *log_file;
char cwd[1024];
void read_history_file() {
	char *line=malloc(512);
	char *path = getenv("HOME");
	size_t message_len = strlen("history_file.txt") + strlen(path) + 1; /* + 1 for terminating NULL */
	char *buffer = (char*) malloc(message_len);
	strcpy(buffer, path);
	int len = strlen(buffer);
	buffer[len] = '/';
	buffer[len + 1] = '\0';
	strcat(buffer, "history_file.txt");

	FILE *file;
	file = fopen(buffer, "r");
	if (!file) {
		printf("Couldn't open file %s for reading.\n", buffer);
		return;
	}
	printf("Opened file %s for reading.\n", buffer);

	//line_number = 0;
	while (fgets(line, sizeof(line), file)) {
		printf("%s", line);
	}
	free(line);
	fclose(file);
}
void init_log_file() {
	char *path = getenv("HOME");
	size_t message_len = strlen("log_file.txt") + strlen(path) + 1; /* + 1 for terminating NULL */
	char *buffer = (char*) malloc(message_len);
	strcpy(buffer, path);
	int len = strlen(buffer);
	buffer[len] = '/';
	buffer[len + 1] = '\0';
	strcat(buffer, "log_file.txt");

	log_file = fopen(buffer, "w");
}
void init_history_file() {

	char *path = getenv("HOME");
	size_t message_len = strlen("history_file.txt") + strlen(path) + 1; /* + 1 for terminating NULL */
	char *buffer = (char*) malloc(message_len);
	strcpy(buffer, path);
	int len = strlen(buffer);
	buffer[len] = '/';
	buffer[len + 1] = '\0';
	strcat(buffer, "history_file.txt");

	history_file = fopen(buffer, "w");

}
void open_history_file() {
	char *path = getenv("HOME");
	size_t message_len = strlen("history_file.txt") + strlen(path) + 1; /* + 1 for terminating NULL */
	char *buffer = (char*) malloc(message_len);
	strcpy(buffer, path);
	int len = strlen(buffer);
	buffer[len] = '/';
	buffer[len + 1] = '\0';
	strcat(buffer, "history_file.txt");
	//printf("%s", buffer);

	history_file = fopen(buffer, "a");
}

FILE* get_history_file() {
	return history_file;
}
void write_in_history_file(char* str) {
	open_history_file();
	fputs(str, history_file);
	close_history_file();
}
void close_history_file() {
	fclose(history_file);
}

void open_log_file() {
	char *path = getenv("HOME");
	size_t message_len = strlen("log_file.txt") + strlen(path) + 1; /* + 1 for terminating NULL */
	char *buffer = (char*) malloc(message_len);
	strcpy(buffer, path);
	int len = strlen(buffer);
	buffer[len] = '/';
	buffer[len + 1] = '\0';
	strcat(buffer, "log_file.txt");
	//printf("%s",buffer);

	log_file = fopen(buffer, "a");
}
void write_in_log_file(char* str) {
	open_log_file();
	fputs(str, log_file);
	close_log_file();
}
FILE* get_log_file() {
	return log_file;
}

void close_log_file() {
	fclose(log_file);
}
