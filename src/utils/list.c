/*
    Alexandre GARCIA - Antoine LEVY - Clémentine Thornary
	game-project - utils/list.c
	Simple linkelist, that can contains integer.
*/
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
// Initialise an empty list
void list_init(List* list){
	list->size = 0;
	list->first_item = NULL;
	list->last_item = NULL;
}
// Return nb items present in list 
int list_size(List* list){
	return list->size;
}
// Append new item in list
// Return 1 if item was appended
// Return 0 if item couldn't have been appended
int list_append(List* list, int item){
	if(list->size == 0){
		list->first_item = malloc(sizeof(Item));
		list->last_item = list->first_item;
		list->first_item->item = item;		
		list->first_item->next_item = NULL;
	}else{
		list->last_item->next_item = malloc(sizeof(Item));
		list->last_item->next_item->item = item;
		list->last_item->next_item->next_item = NULL;
		list->last_item =  list->last_item->next_item;
	}
	list->size++;
	return 1;
}	
// Insert new item in list at a certain position
// Position must be inside the list range
// Return 1 if item was inserted
// Return -1 if out of range
int list_insert(List* list, int item, int pos){
	if(pos > list->size || pos < 0){
		return -1;
	}
	if(list->size == 0 || pos == list->size){
		list_append(list, item);
		return 1;
	}else{	
			Item* new_item_pt = malloc(sizeof(Item));
			new_item_pt->item = item;
		if(pos == 0){
			new_item_pt->next_item = list->first_item;
			list->first_item = new_item_pt;
		}else{			
			Item* tmp_item_pt = list->first_item;
			for (int i = 0; i < pos-1; i++){
				tmp_item_pt = tmp_item_pt->next_item;
			}
			new_item_pt->next_item = tmp_item_pt->next_item;
			tmp_item_pt->next_item = new_item_pt;

			if(pos == 0){
				list->first_item = tmp_item_pt;
			}
		}
		list->size++;
		return 1;
	}
}
// Delete the item from list
// Return 1 if a item is deleted
// Return 0 if no item is deleted (because not in list)
// Return -1 if list is empty
int list_delete(List* list, int item){
	if(list->size == 0){
		printf("List is empty\n");
		return -1;
	}

	Item* pt = list->first_item;
	Item* previous_item = list->first_item;

	for (int i = 0; i < list->size; ++i){
		if(pt->item == item){
			if(i == 0){
				if(list->size == 1){
					list->first_item = NULL;
					list->last_item = NULL;
					list->size = 0;
				}else{
					list->first_item = list->first_item->next_item;
					list->size--;
				}
			}else if (i == list->size-1){
				list->last_item = previous_item;
				list->size--;
			}else{
				previous_item->next_item = pt->next_item;
				list->size--;
			}
			free(pt);
			return 1;
		}
		if(i != 0){
			previous_item = previous_item->next_item;
		}
		pt = pt->next_item;
	}
	return 0;
}
// Return the item at position pos in list
// Return -1 if position i out of range
int list_get(List* list, int pos){
	if(pos > (list->size)-1 || pos < 0){
		return -1;
	}
	Item* return_item = list->first_item;
	while( pos != 0){
		return_item = return_item->next_item; 
		pos--;
	}
	return return_item->item;
}
// Check if list contains item
// Return the position of item is present in list
// Return -1 if not present or if list is empty
int list_contains(List* list, int item){
	if(list->size == 0){
		return -1;
	}
	Item* item_pt = list->first_item;
	for (int i = 0; i < list->size; i++){
		if(item_pt->item == item){
			return i;
		}
		if(i != ((list->size)-1)){ // NOT SURE
			item_pt = item_pt->next_item;
		}
	}
	return -1;
}
int list_empty(List* list){
	// TODO
	return 0;
}	
// Print list
void list_print(List* list){
	int i;
	Item* item_pt = list->first_item;
	printf("---------------------------------------\n");
	printf("list size = %d\n", list->size);
	for(i=0;i<list->size; i++){
		printf("position %d : %d\n", i, item_pt->item );
		if(item_pt->next_item){
			item_pt=item_pt->next_item;
		}
	}
	printf("---------------------------------------\n");
}
