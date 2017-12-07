#include "variables.h"
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIZE 500

struct DataItem {
	char* data;
	char* key;
};

struct DataItem* hashArray[SIZE];
struct DataItem* dummyItem;
struct DataItem* item;
void init() {
	dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
	dummyItem->data = NULL;
	dummyItem->key = NULL;
}
int hashCode(char* key) {
	unsigned long hash = 5381;
	int c;

	while (c = *key++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash % SIZE;
}
char* lookup_variable(char* key) {
	//get the hash
	int hashIndex = hashCode(key);

	//move in array until an empty
	while (hashArray[hashIndex] != NULL) {

		if (!strcmp(hashArray[hashIndex]->key , key))
			return hashArray[hashIndex]->data;

		//go to next cell
		++hashIndex;

		//wrap around the table
		hashIndex %= SIZE;
	}

	return getenv(key);
}

void set_variable(char* key, char* data) {

	struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
	item->data = strdup(data);
	item->key = strdup(key);

	//get the hash
	int hashIndex = hashCode(key);

	//move in array until an empty or deleted cell
	while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != NULL) {
		if(!strcmp(hashArray[hashIndex]->key,key))break;
		//go to next cell
		++hashIndex;

		//wrap around the table
		hashIndex %= SIZE;
	}

	hashArray[hashIndex] = item;
}

void print_all_variables(void) {

	int i = 0;

	printf("key\tvalue\n");
	for (i = 0; i < SIZE; i++) {
		if(hashArray[i] != NULL)
			printf("%s\t%s\n", hashArray[i]->key, hashArray[i]->data);

	}
	printf("\n");
}
