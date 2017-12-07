
CC=gcc
CFLAGS=-I.
DEPS = main.h command_parser.h commands.h environment.h file_processing.h variables.h
OBJ = main.o command_parser.o commands.o environment.o file_processing.o variables.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shell: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
