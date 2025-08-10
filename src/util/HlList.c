/*
 * kilobyte-disk 2025
 * HlList.c
 *
 * Purpose:
 * An integer array that can be reorganized
 * easily.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "HlList.h"

int ACTIVE_HLLISTS = 0;

/* WARNING:
 * Every HlList is dynamically allocated
 * on the heap! Remember to free() these later.
 */
struct HlList HLLIST_Init(int elements)
{
	printf("[HlList]: Creating new HlList [%i]\n", elements);

	size_t size = (sizeof(int) * elements);
	int *data = malloc(size);

	/* Initialize all elements at zero */
	int i = 0;
	for (i = 0; i < elements; i++) {
		data[i] = 0;
	}

	struct HlList list;
	list.active = true;
	list.id = ACTIVE_HLLISTS;
       	
	ACTIVE_HLLISTS++;

	list.elements = elements;
	list.data = data;

	return list;
}

void HLLIST_DeInit(struct HlList *list)
{
	if (!list->active) {
		printf("[HlList]: List already DeInit!\n");
		return;
	}

	printf("[HlList]: Freeing list %i\n", list->id);

	free(list->data);
}

int HLLIST_Resize(struct HlList *list, int new_size)
{
	if (!list->active) {
		printf("[HlList]: [ERROR] List has been freed!\n");
		return 1;
	}

	if (new_size < 0) {
		printf("[HlList]: ERROR Cannot Resize, new_size less than 0.\n");
		return 1;
	}

	/* Allocate new block of memory */
	size_t size = (sizeof(int) * new_size);
	int *data = malloc(size);

	/* Copy elements over */
	int copy_elements = list->elements;
	if (new_size < list->elements) {
		printf("[HlList]: Downsizing, will truncate %i elements!\n", (list->elements - new_size));

		copy_elements = size;
	}

	int i = 0;
	for (i = 0; i < copy_elements; i++) {
		data[i] = list->data[i];
	}

	/* Free the old memory */
	free(list->data);

	/* Set the data pointer to the new memory address */
	list->data = data;

	return 0;
}

int HLLIST_Swap(struct HlList *list, int target, int dest)
{
	if (!list->active) {
		printf("[HlList]: [ERROR] List has been freed!\n");
		return 1;
	}

	if ((target > list->elements) || (target < 0)) {
		printf("[HlList]: [ERROR] Cannot Swap, target out of bounds.\n");
		return 1;
	}

	if ((dest > list->elements) || (dest < 0)) {
		printf("[HlList]: [ERROR] Cannot Swap, destination out of bounds.\n");
		return 1;
	}

	int temp = list->data[dest];

	/* Switcharoo */
	list->data[dest] = list->data[target];
	list->data[target] = temp;

	return 0;
}

int HLLIST_MoveUp(struct HlList *list, int target)
{
	int result = HLLIST_Swap(list, target, (target + 1));

	return result;
}

int HLLIST_MoveDown(struct HlList *list, int target)
{
	int result = HLLIST_Swap(list, target, (target - 1));

	return result;
}
