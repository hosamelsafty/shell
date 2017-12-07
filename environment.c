#include "environment.h"
#include "variables.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void init();
void init_history_file();
void init_log_file();
void setup_environment(void) {
	init();
	init_history_file();
	init_log_file();

}
