/*
 * kilobyte-disk 2025
 * FlashState.c
 *
 * Purpose:
 * The flashcard portion of the program. 
 * Display each card to the user, and allow them
 * to flip the card to see the rear side.
 */

#include <stdio.h>
#include "raylib.h"

#include "Helium.h"
#include "GLOBALS.h"
#include "HlData.h"

struct Data {
	struct HlDeck deck;
};

struct Data FlashState;


/*
 * Initialize the state
 */

int FLASHSTATE_Init(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[FlashState]: Entering FLASHSTATE\n");

	int FILENAME_LIMIT = 100; /* TODO: make loading lib for this */
	char path[FILENAME_LIMIT];

	sprintf(path, "./data/cards/%s", CORE->GLOBALS->CurrentDeck);

	printf("Loading deck %s\n", path);
	
	FlashState.deck = DATA_ReadHlDeck(path);

	return 0;
}



/*
 * Cleanup the state
 */

int FLASHSTATE_DeInit(struct HlCore *CORE, float delta_time)
{
	printf("%s", "[FlashState]: Exiting FLASHSTATE\n");

	return 0;
}



/*
 * Update
 * NOTE: This runs BEFORE BeginDraw(); is called.
 */
int FLASHSTATE_Update(struct HlCore *CORE, float delta_time)
{
	/* Update values */

	return 0;
}



/*
 * Render
 * This is where things should be rendered.
 */
int FLASHSTATE_Render(struct HlCore *CORE, float delta_time)
{
	/* Render things */
	Color bgcolor = { 66, 72, 82, 255 };
	ClearBackground(bgcolor);

	return 0;
}
