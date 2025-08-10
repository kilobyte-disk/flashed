#ifndef HLLIST_H
#define HLLIST_H

#include <stdbool.h>

struct HlList {
	bool active;

	int id;
	int elements;

	int *data;
};

extern struct HlList HLLIST_Init(int elements);
extern void HLLIST_DeInit(struct HlList *list);
extern int HLLIST_Resize(struct HlList *list, int new_size);

extern int HLLIST_Swap(struct HlList *list, int target, int dest);
extern int HLLIST_MoveUp(struct HlList *list, int target);
extern int HLLIST_MoveDown(struct HlList *list, int target);


#endif
