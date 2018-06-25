/*
    Antoine LEVY - Clémentine Thornary
	game-project - utils/list.h
*/
#ifndef LIST_H
#define LIST_H
typedef struct Item {
	struct Item* next_item;
	int item;
} Item;

typedef struct List {
	Item* first_item;
	Item* last_item;
	int size;
} List;

void list_init(List* list);
int list_size(List* list);
int list_append(List* list, int item);
int list_insert(List* list, int item, int pos);
int list_delete(List* list, int item);
int list_get(List* list, int pos);
int list_contains(List* list, int item);
int list_empty(List* list);
void list_print(List* list);
#endif // LIST_H