/*
    Antoine LEVY - Clémentine Thornary
	game-project - test/test_list.c
	Version 1.0 - 09/04/2018
*/
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include "../utils/list.h"
int test_list();

int main(int argc, char** argv){
	if(test_list() == 1){
		printf("All tests passed !\n");
	}
	return 0;
}

int test_list(){
   	List* list = malloc(sizeof(List));
	list_init(list);
	// Check for get and contains in empty list
	assert(list_get(list, 0) == -1); // List : empty
	assert(list_contains(list, 1) == -1); // List : empty
	// Check for append in empty list
	assert(list_append(list, 1) == 1) ; // List : 1
	// Check for get a pos where value is present
	assert(list_get(list, 0) == 1); // List : 1
	// Check for get a pos out of range
	assert(list_get(list, 1) == -1); // List : 1
	// Check for contain a value that is present
	assert(list_contains(list, 1) == 0); // List : 1
	// Check for delete a value that is absent
	assert(list_delete(list, 86) == 0); // List : 1
	// Check for delete a value that is present
	assert(list_delete(list, 1) == 1); // List : 1
	// Check for delete a value in empty list
	assert(list_delete(list, 1) == -1); // List : empty
	// Check for insert
	assert(list_insert(list, 1, 0) == 1); // List : empty
	assert(list_insert(list, 2, 1) == 1); // List : 1
	assert(list_insert(list, 3, 0) == 1); // List : 1-2
	assert(list_insert(list, 4, 2) == 1); // List : 3-1-2
	// Check for insert out of range
	assert(list_insert(list, 16, 64) == -1); // List : 3-1-4-2
	// Check for append to existing (and not empty) list
	assert(list_append(list, 5) == 1); // List : 3-1-4-2-5
	assert(list_get(list, 1) == 1); // List : 3-1-4-2-5
	assert(list_get(list, 2) == 4); // List : 3-1-4-2-5
	assert(list_get(list, 42) == -1); // List : 3-1-4-2-5
	assert(list_contains(list, 1) == 1); // List : 3-1-4-2-5
	assert(list_contains(list, 2) == 3); // List : 3-1-4-2-5
	assert(list_contains(list, 42) == -1); // List : 3-1-4-2-5
	// Check for delete
	assert(list_delete(list, 2) == 1); // List : 3-1-4-2-5
	assert(list_delete(list, 1) == 1); // List : 3-1-4-5
	assert(list_delete(list, 3) == 1); // List : 3-4-5
	assert(list_delete(list, 42) == 0); // List : 4-5
	assert(list_delete(list, 4) == 1); // List : 4-5
	assert(list_delete(list, 5) == 1); // List : 5
	// Check for delete in empty list
	assert(list_delete(list, 42) == -1); // List : empty
	assert(list_get(list, 0) == -1); // List : empty
	assert(list_contains(list, 1) == -1); // List : empty
	return 1;
}