#include "commands.h"
#include  <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include  <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
void read_history_file();
void write_in_log_file();
void set_variable(const char* key, const char* value);
void print_all_variables(void);
char** str_split(char* command, char* spiliter);
char* lookup_variable(char* key);

char * finding(char **path, char *command, char *buffer) {

	int i = 0;

	strcpy(buffer, *path);
	int len = strlen(buffer);
	buffer[len] = '/';
	buffer[len + 1] = '\0';
	strcat(buffer, path[0]);
	while (path[i]) {
		strcpy(buffer, path[i++]);
		int len = strlen(buffer);
		buffer[len] = '/';
		buffer[len + 1] = '\0';
		strcat(buffer, command);

		i++;
		if (!access(buffer, X_OK))
			return buffer;

	}

	return 0;
}
void cd(const char* path) {
	char directory[1024];

	getcwd(directory, sizeof(directory));

	if (!strcmp(path, "")) {
		if (chdir(lookup_variable("HOME")) == 0) {
			getcwd(directory, sizeof(directory));

		}
	} else if (chdir(path) == 0) {
		getcwd(directory, sizeof(directory));

	} else {
		printf("bash: cd: %s: No such file or directory\n", path);
	}

}
int check_eqaution(char* command) {
	for (int i = 0; i < strlen(command); ++i) {

		if (isalpha(command[i]) || command[i] == '_')
			while (isalpha(command[i]) || isdigit(command[i])
					|| command[i] == '_') {
				i++;
			}
		if (command[i] == '=')
			return 1;
		else
			return 0;

	}
	return 0;
}
void echo(const char* message) {
	int flag = 0;

	if(message!=NULL) for (int i = 0; i < strlen(message); ++i) {
		if (message[i] == '\"' && !flag) {
			flag = 1;
			i++;
		} else if (message[i] == '\"' && flag) {
			flag = 0;
			i++;
		}
		if (flag) {
			printf("%c", message[i]);
		} else {
			if (message[i] != '\0' && message[i] != ' ' && message[i] != '\t'
					&& message[i] != '\n')
				printf("%c", message[i]);
		}
	}
	printf("\n");
}

void execute(char **argv, int back) {

	pid_t pid;
	int status;

	if ((pid = fork()) < 0) { /* fork a child process           */
		printf("*** ERROR: forking child process failed\n");
		exit(1);
	} else if (pid == 0&& strcmp(argv[0], "cd")) { /* for the child process:         */
		write_in_log_file("child created\n");

		if (argv[0][0] != '/') {
			char **spilit = str_split(lookup_variable("PATH"), ":");
			size_t message_len = strlen(argv[0]) + strlen(*spilit) + 1; /* + 1 for terminating NULL */
			char *buffer = (char*) malloc(message_len);
			char* test = finding((char **) spilit, argv[0], buffer);
			if (execv(test, argv) < 0) { /* execute the command  */
				printf("*** ERROR: exec failed\n");

				exit(1);
			}
            free(buffer);



		} else {
			//strtok(argv[0], "\n");
			if (execv(argv[0], argv) < 0) { /* execute the command  */
				printf("*** ERROR: exec failed\n");
				exit(1);
			}
		}

	} else { /* for the parent:      */
		if (!strcmp(argv[0], "echo")) {
			kill(pid, SIGKILL);

			echo(argv[1]);
		}
		if (!strcmp(argv[0], "history")) {
			kill(pid, SIGKILL);
			read_history_file();
		}
		if (!strcmp(argv[0], "cd")) {
			kill(pid, SIGKILL);
			if (argv[1] == NULL)
				cd("");
			else if (argv[2] == NULL)
				cd(argv[1]);
			else
				printf("error in parameter\n");
		} else if (!strcmp(argv[0], "export")) {
			kill(pid, SIGKILL);
			char **eqaution = str_split(argv[1], "=");
			set_variable(eqaution[0], eqaution[1]);
			print_all_variables();
		} else if (check_eqaution(argv[0])) {
			kill(pid, SIGKILL);
			char **eqaution = str_split(argv[0], "=");
			set_variable(eqaution[0], eqaution[1]);
			print_all_variables();
		} else if (!back) {
			while (wait(&status) != pid)
				/* wait for completion  */
				;
		} else {
			sleep(1);
		}
	}
}

