#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "variables.h"
#include "commands.h"
#include "command_parser.h"

typedef enum {
	false = 0, true = 1
} bool;

void shell_loop(bool input_from_file);
void start(bool read_from_file);
void setup_environment(void);
void cd(const char* path);
char **file_name;
int main(int argc, char **argv) {
	file_name = (char**) malloc(sizeof(char*) * 50);
	file_name = argv;

	setup_environment();
	if (argc > 1) {

		//file_name = argv;
		start(true);
	} else {
		start(false);
	}

	return 0;
}

void start(bool read_from_file) {
	cd(""); // let shell starts from home

	if (read_from_file) {
		// file processing functions should be called from here

		shell_loop(true);
	} else {
		shell_loop(false);
	}
}

void shell_loop(bool input_from_file) {
	bool from_file = input_from_file;
	char line[1024]; /* the input line                 */
	char *command[64]; /* the command line argument      */

	while (1) {
		if (from_file) {
			char *name = file_name[1];
			FILE *file;
			file = fopen(name, "r");
			if (!file) {
				printf("Couldn't open file %s for reading.\n", name);
				return;
			}
			printf("Opened file %s for reading.\n", name);

			//line_number = 0;
			while (fgets(line, sizeof(line), file)) {
				printf("%s\n\n", line);
				//fflush(stdout);
				// ++line_number;
				/* note that the newline is in the buffer */

				strtok(line, "\n");
				int back=parse(line, command); /*   parse the line               */
				if (strcmp(command[0], "exit") == 0) /* is it an "exit"?     */
					exit(0); /*   exit if it is*/
				execute(command,back); /* otherwise, execute the command */


			}
			fclose(file);
			from_file = false;

		} else {
			char directory[1024];
			getcwd(directory, sizeof(directory));
			printf("%s -> ", directory); /*   display a prompt             */
			fflush(stdout);
			if(fgets(line, 1024, stdin) == NULL)break; /*   read in the command line     */
			strtok(line, "\n");

			printf("\n");
			if(line[0]=='#'||line[0]=='\0'||line[0]=='\n'||line[0]==' ')continue;
			int back=parse(line, command); /*   parse the line               */

			if (strcmp(command[0], "exit") == 0) /* is it an "exit"?     */
				exit(0); /*   exit if it is                */

			execute(command, back); /* otherwise, execute the command */

		}

	}
}
